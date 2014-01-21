/****************************************************************************
The MIT License (MIT)

Copyright (c) 2014 LTU Aero Design

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Metro.h>
#include <TinyGPS.h>
#include <AeroC2.h>

// Pins
const int AIRSPEED_SENSOR = A0;
const int RADIO_EN = 2;
const int LED = 13;

// ADC
const int ADC_RES = 12;
const float ADC_MAX = pow(2, ADC_RES) - 1;
const float VREF = 3.284;

// Constants
const int SYS = 100;
const int VER = 1;

// Globals
float temperature;
int calibrated_pressure;

// Comms
unsigned int len;
byte buf[AEROC2_MAX_MESSAGE_LEN];
aeroc2_message_t msg;

// Devices
Adafruit_BMP085_Unified bmp(10085);
MPU6050 mpu;
TinyGPS gps;

// Soft timers
Metro metro_100ms(100);
Metro metro_500ms(500);
Metro metro_1000ms(1000);

void setup() {
  // Initialize systems
  analogReadResolution(ADC_RES);
  analogReadAveraging(8);
  Wire.begin();
  Serial1.begin(38400);
  Serial2.begin(9600);
  pinMode(RADIO_EN, OUTPUT);
  pinMode(LED, OUTPUT);
  delay(100);
  
  // Set default GPIO states
  digitalWrite(RADIO_EN, HIGH);
  digitalWrite(LED, LOW);
  
  // Send a boot message
  aeroc2_msg_boot_pack(&msg, SYS, 0, VER);
  len = aeroc2_message_to_buffer(buf, &msg);
  Serial1.write(buf, len);
  
  // Initialize all I2C devices
  if (!bmp.begin()) {
      while (true) {
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
    }
  }
  
  mpu.initialize();
  while (!mpu.testConnection()) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
  
  // Warmup time
  delay(500);
  
  // Perform first time calibrations
  calibrate_pressure();
}

void loop() {
  // High execution rate
  if (metro_100ms.check()) {
    send_imu_data();
  }
  
  // Medium execution rate
  if (metro_500ms.check()) {
    send_altitude();
    send_airspeed();
  }
  
  // Low execution rate
  if (metro_1000ms.check()) {
    send_keep_alive();
    send_gps_data();
  }
}

void calibrate_pressure() {
  sensors_event_t event;
  bmp.getEvent(&event);
  calibrated_pressure = event.pressure / 100.0;
}

void send_keep_alive() {
  aeroc2_msg_keep_alive_pack(&msg, SYS, 0, AEROC2_FIXED_WING, AEROC2_MANUAL);
  len = aeroc2_message_to_buffer(buf, &msg);
  Serial1.write(buf, len);
}

void send_airspeed() {
  const float a0 = 340.29;      // Speed of sound at sea level (m/s)
  const float t0 = 288.15;      // Standard sea level temperature (K)
  const float t = temperature;  // Measured temperature (K)
  float qc_over_p;              // Measured differential pressure
  float vt;                     // True velocity
  
  // Read sensor, convert to pressure (kPa)
  // P(at 5V) = Vo - (2.5 +- 6.25%)
  // P(at 3V) = Vo - (1.5 +- 3.75%)
  qc_over_p = ((float)analogRead(AIRSPEED_SENSOR) *
               (VREF / ADC_MAX)) - (1.5 - 0.2);
  
  // Compute true airspeed (m/s)
  if (qc_over_p <= 0.0) {
    vt = 0.0;
  } else {
    vt = a0 * sqrt(5.0 * (pow(qc_over_p + 1, 2.0 / 7.0) - 1) *
      (t / t0));
  }
  
  // Send data
  aeroc2_msg_airspeed_pack(&msg, SYS, 150, vt);
  len = aeroc2_message_to_buffer(buf, &msg);
  Serial1.write(buf, len);
}

void send_imu_data() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t mx, my, mz;
  
  // Read sensor
  mpu.getMotion9(&ax, &ay, &az,
                 &gx, &gy, &gz,
                 &mx, &my, &mz);
  
  // Send data
  aeroc2_msg_imu_raw_pack(&msg, SYS, 200,
                          ax, ay, az,
                          gx, gy, gz,
                          mx, my, mz);
  len = aeroc2_message_to_buffer(buf, &msg);
  Serial1.write(buf, len);
}

void send_gps_data() {
  boolean new_data = false;
  char c;
  float lat;
  float lon;
  unsigned long age;
  
  while (Serial2.available()) {
    c = Serial2.read();
    if (gps.encode(c)) {
      new_data = true;
    }
  }
  
  if (new_data) {
    gps.f_get_position(&lat, &lon, &age);
    if (TinyGPS::GPS_INVALID_AGE == age) {
      // No GPS signal
      // TODO Send critical message
    } else if (5000 < age) {
      // GPS signal lost
      // TODO Send critical message
    } else {
      // Valid GPS position
      aeroc2_msg_gps_position_pack(&msg, SYS, 110, lat, lon, gps.f_altitude());
      len = aeroc2_message_to_buffer(buf, &msg);
      Serial1.write(buf, len);
    }
  }
}

void send_altitude() {
  float alt;
  sensors_event_t event;
  
  // Read sensor
  bmp.getEvent(&event);
  bmp.getTemperature(&temperature);
  alt = bmp.pressureToAltitude(calibrated_pressure,
                               event.pressure,
                               temperature);
  
  // Send data
  aeroc2_msg_altitude_pack(&msg, SYS, 160, alt);
  len = aeroc2_message_to_buffer(buf, &msg);
  Serial1.write(buf, len);
}
