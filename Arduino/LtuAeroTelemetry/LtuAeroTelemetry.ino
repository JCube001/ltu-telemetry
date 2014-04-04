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
*****************************************************************************

LTU Aero software projects are hosted on the following GitHub.
https://github.com/JCube001

*****************************************************************************

Sensors and devices
  Bosch BMP085       - Pressure and temperature sensor
  InvenSense MPU9150 - 9DoF orientation sensor
  uBlox NEO-6M       - GPS module
  TI HP4067          - 16 pin analog multiplexer
  APC802             - Radio communication module
  AttoPilot 90A      - Voltage and current sensor
  MPXV7002DP         - Differential pressure sensor

*****************************************************************************

Hookup guide

Teensy 3.0
http://www.pjrc.com/teensy/pinout.html
   GND ---- Power GND
     0 ---- Radio Tx
     1 ---- Radio Rx
     2 ---- Analog Mux S0
     3 ---- Analog Mux S1
     4 ---- Analog Mux S2
     5 ---- Analog Mux S3
     6
     7
     8
     9 ---- GPS Tx
    10 ---- GPS Rx
    11
    12
    13
    14 ---- Analog Mux SIG
    15
    16
    17
    18 ---- I2C Bus SDA
    19 ---- I2C Bus SCL
    20
    21
    22
    23
  3.3V
  AGND
   VIN ---- Power 5V

*****************************************************************************

Communications protocol

All data will be converted to engineering (SI) units and then prefixed with
a dollar sign '$' and a three letter code specifiying the type of message. A
data value will proceed the type information. A character 'T' will proceed
the data and itself be proceeded by a timestamp value. The timestamp should
be some sort of linear measure of time (e.g. millis()). The full message will
be transmitted as ASCII text and each individual message will end with a
linefeed character '\n'. Note that CRLF ("\r\n") line endings are also allowed.

The type codes are as follows:
  AIN - Amperes in (amps)
  ALT - Altitude (meters)
  AQW - Attitude (quaternion w element)
  AQX - Attitude (quaternion x element)
  AQY - Attitude (quaternion y element)
  AQZ - Attitude (quaternion z element)
  CLB - Climb rate (meters / second)
  HDG - Heading (degrees from north clockwise)
  LAT - GPS latitude (degrees)
  LON - GPS longitude (degrees)
  SPD - Airspeed (meters / second)
  TMP - Temperature (degrees Celsius)
  VIN - Voltage in (volts)
  
Examples
  $SPD0.182T0\n
  $AQX0.343111T1\n
  $AQW0.65423T204\n
  $AQY0.123T204\n
  $AQZ-0.005T204\n
  $ALT56.72T204\n

****************************************************************************/

#include <Wire.h>
#include <I2Cdev.h>
#include <TinyGPS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <MPU6050_9Axis_MotionApps41.h>

// MCU pins
const int ANALOG_MUX_SIG = A0;
const int ANALOG_MUX_S0 = 2;
const int ANALOG_MUX_S1 = 3;
const int ANALOG_MUX_S2 = 4;
const int ANALOG_MUX_S3 = 5;
const int LED = 13;

// Analog multiplexer pins
const int AIRSPEED_SENSOR = 0;
const int AMPERE_SENSOR = 1;
const int VOLTAGE_SENSOR = 2;

// ADC
const int ADC_RESOLUTION = 12;
const int ADC_MAX = (int)pow(2.0, (float)ADC_RESOLUTION) - 1.0;
const float VREF = 3.284;

// Globals
unsigned long previous_time = 0ul;
int ground_level_pressure;
unsigned short dmp_packet_size;

// Sensors
Adafruit_BMP085 *bmp = new Adafruit_BMP085;
MPU6050 *mpu = new MPU6050;
TinyGPS *gps = new TinyGPS;

void setup() {
  // ADC
  analogReadResolution(ADC_RESOLUTION);
  analogReadAveraging(5);
  
  // I2C
  Wire.begin();
  
  // Debug info
  Serial.begin(115200);
  
  // Comm radio
  Serial1.begin(38400);
  
  // GPS
  Serial2.begin(9600);
  
  // GPIO
  pinMode(ANALOG_MUX_S0, OUTPUT);
  pinMode(ANALOG_MUX_S1, OUTPUT);
  pinMode(ANALOG_MUX_S2, OUTPUT);
  pinMode(ANALOG_MUX_S3, OUTPUT);
  pinMode(LED, OUTPUT);
  
  // Turn on LED once system has been given time to initialize
  delay(5000);
  digitalWrite(LED, HIGH);
  
  // Test communication with sensors
  Serial.println(F("Testing sensors..."));
  
  if (!bmp->begin()) {
    Serial.println(F("Pressure sensor failed"));
    while (true);
  }
  Serial.println(F("Pressure sensor OK"));
  
  mpu->initialize();
  while (!mpu->testConnection()) {
    Serial.println(F("Unable to connect to attitude sensor, retrying"));
    delay(100);
  }
  Serial.println(F("Attitude sensor OK"));
  
  if (mpu->dmpInitialize() != 0) {
    Serial.println(F("DMP failed"));
    while (true);
  }
  Serial.println(F("DMP OK"));
  
  // Turn on the DMP
  mpu->setDMPEnabled(true);
  dmp_packet_size = mpu->dmpGetFIFOPacketSize();
  
  // Store the pressure at ground level
  ground_level_pressure = bmp->readPressure();
  
  // Print a blank line
  Serial.println();
}

void loop() {
  unsigned long current_time = millis();
  unsigned long delta_time = current_time - previous_time;
  
  if (delta_time >= 200ul) {
    previous_time = current_time;
    
    // AQW, AQX, AQY, AQZ
    send_attitude();
    
    // SPD
    send_airspeed();
    
    // AIN
    send_ampere_measure();
    
    // ALT, CLB
    send_altitude();
    
    // HDG
    send_heading();
    
    // VIN
    send_voltage_measure();
    
    // LAT, LON
    send_position();
    
    // TMP
    send_temperature();
    
    // Print blank line in debug output
    Serial.println();
  }
}

/**
 * https://www.sparkfun.com/products/9028
 */
void send_ampere_measure() {
  float amps;
  
  // Read the current sensor and convert to amperes
  select_adc_mux(AMPERE_SENSOR);
  amps = (float)analogRead(ANALOG_MUX_SIG) / 23.0193;
  
  // Print debug message
  Serial.print("Current: ");
  Serial.print(amps, 3);
  Serial.println(" A");
  
  // Send comm message
  Serial1.print("$AIN");
  Serial1.print(amps, 3);
  Serial1.print("T");
  Serial1.println(millis());
}

/**
 * http://learn.adafruit.com/bmp085/using-the-bmp085
 */
void send_altitude() {
  unsigned long current_time = millis();
  static unsigned long previous_time;
  static float previous_altitude;
  float current_altitude;
  float climb;
  
  // Read pressure sensor
  current_altitude = bmp->readAltitude(ground_level_pressure);
  
  // Determine climb rate
  climb = (current_altitude - previous_altitude)
    / (1000.0 / (float)(current_time - previous_time));
  
  previous_time = current_time;
  previous_altitude = current_altitude;
  
  // Print debug message
  Serial.print("Altitude: ");
  Serial.print(current_altitude, 6);
  Serial.println(" m");
  
  Serial.print("Climb rate: ");
  Serial.print(climb, 6);
  Serial.println(" m/s");
  
  // Send comm message
  Serial1.print("$ALT");
  Serial1.print(current_altitude, 6);
  Serial1.print("T");
  Serial1.println(current_time);
  
  Serial1.print("$CLB");
  Serial1.print(climb, 6);
  Serial1.print("T");
  Serial1.println(current_time);
}

/**
 * https://github.com/sparkfun/MPU-9150_Breakout
 */
void send_attitude() {
  const unsigned long current_time = millis();
  unsigned short count = mpu->getFIFOCount();
  Quaternion q;
  byte buffer[64];
  
  // Reset in order to continue cleanly
  if (count == 1024) {
    mpu->resetFIFO();
    count = mpu->getFIFOCount();
  }
  
  // Wait for the correct available data length
  while (count < dmp_packet_size) count = mpu->getFIFOCount();
  
  // Read a packet from the sensor's FIFO
  mpu->getFIFOBytes(buffer, dmp_packet_size);
  
  // Get a quaternion from the buffer
  mpu->dmpGetQuaternion(&q, buffer);
  
  // Print debug message
  Serial.print("Attitude:");
  Serial.print(" W=");
  Serial.print(q.w, 4);
  Serial.print(" X=");
  Serial.print(q.x, 4);
  Serial.print(" Y=");
  Serial.print(q.y, 4);
  Serial.print(" Z=");
  Serial.println(q.z, 4);
  
  // Send comm message
  Serial1.print("$AQW");
  Serial1.print(q.w, 6);
  Serial1.print("T");
  Serial1.println(current_time);
  
  Serial1.print("$AQX");
  Serial1.print(q.x, 6);
  Serial1.print("T");
  Serial1.println(current_time);
  
  Serial1.print("$AQY");
  Serial1.print(q.y, 6);
  Serial1.print("T");
  Serial1.println(current_time);
  
  Serial1.print("$AQZ");
  Serial1.print(q.z, 6);
  Serial1.print("T");
  Serial1.println(current_time);
}

/**
 * http://www.adafruit.com/datasheets/AN203_Compass_Heading_Using_Magnetometers.pdf
 */
void send_heading() {
  int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
  float heading;
  
  // Read sensor
  mpu->getMotion9(&ax, &ay, &az,
                  &gx, &gy, &gz,
                  &mx, &my, &mz);
  
  // Compute heading
  // Note: Does not correct for local magnetic delineation
  if (my > 0) {
    heading = 90.0 - (atan((float)mx / (float)my) * (180.0 / PI));
  } else if (my < 0) {
    heading = 270.0 - (atan((float)mx / (float)my) * (180.0 / PI));
  } else if (my == 0 && mx < 0) {
    heading = 180.0;
  } else if (my == 0 && mx > 0) {
    heading = 0.0;
  }
  
  // Print debug message
  Serial.print("Heading: ");
  Serial.print(heading, 2);
  Serial.println(" deg");
  
  // Send comm message
  Serial1.print("$HDG");
  Serial1.print(heading, 2);
  Serial1.print("T");
  Serial1.println(millis());
}

/**
 * http://arduiniana.org/libraries/tinygps/
 */
void send_position() {
  const unsigned long current_time = millis();
  unsigned long age;
  float lat;
  float lon;
  
  while (Serial2.available()) {
    if (gps->encode(Serial2.read())) {
      gps->f_get_position(&lat, &lon, &age);
      
      if (age == TinyGPS::GPS_INVALID_AGE) {
        // No signal
      } else if (age > 5000) {
        // Signal lost
      } else {
        // Valid signal
        // Print debug message
        Serial.print("Latitude: ");
        Serial.println(lat, 6);
        
        Serial.print("Longitude: ");
        Serial.println(lon, 6);
        
        // Send comm message
        Serial1.print("$LAT");
        Serial1.print(lat, 6);
        Serial1.print("T");
        Serial1.println(current_time);
        
        Serial1.print("$LON");
        Serial1.print(lon, 6);
        Serial1.print("T");
        Serial1.println(current_time);
      }
    }
  }
}

/**
 * http://en.wikipedia.org/wiki/Airspeed
 */
void send_airspeed() {
  const float a0 = 340.29;                 // Speed of sound at sea level (m/s)
  const float t0 = 15.0;                   // Standard sea level temperature (C)
  const float t = bmp->readTemperature();  // Measured temperature (C)
  float qc_over_p;                         // Measured differential pressure
  float vt;                                // True velocity (m/s)
  
  // Read the differential pressure sensor
  // P(at 5V) = Vo - (2.5 +- 6.25%)
  // P(at 3V) = Vo - (1.5 +- 3.75%)
  select_adc_mux(AIRSPEED_SENSOR);
  qc_over_p = ((float)analogRead(ANALOG_MUX_SIG) *
    (VREF / ADC_MAX)) - (1.5 - 0.2);
  
  // Determine true airspeed
  if (qc_over_p <= 0.0) {
    vt = 0.0;
  } else {
    vt = a0 * sqrt(5.0 * (pow(qc_over_p + 1.0, 2.0 / 7.0) - 1.0)
      * (t / t0));
  }
  
  // Print debug message
  Serial.print("Airspeed: ");
  Serial.print(vt, 6);
  Serial.println(" m/s");
  
  // Send comm message
  Serial1.print("$SPD");
  Serial1.print(vt, 6);
  Serial1.print("T");
  Serial1.println(millis());
}

/**
 * https://www.sparkfun.com/products/9028
 */
void send_voltage_measure() {
  float voltage;
  
  // Read the voltage sensor and convert to volts
  select_adc_mux(VOLTAGE_SENSOR);
  voltage = (float)analogRead(ANALOG_MUX_SIG) / 45.4082;
  
  // Print debug message
  Serial.print("Voltage: ");
  Serial.print(voltage, 3);
  Serial.println(" V");
  
  // Send comm message
  Serial1.print("$VIN");
  Serial1.print(voltage, 3);
  Serial1.print("T");
  Serial1.println(millis());
}

/**
 * http://learn.adafruit.com/bmp085/using-the-bmp085
 */
void send_temperature() {
  float t;
  
  // Read the temperature sensor
  t = bmp->readTemperature();
  
  // Print debug message
  Serial.print("Temperature: ");
  Serial.print(t, 4);
  Serial.println(" C");
  
  // Send comm message
  Serial1.print("$TMP");
  Serial1.print(t, 4);
  Serial1.print("T");
  Serial1.println(millis());
}

void select_adc_mux(int pin) {
  if (pin & 0x1) {
    digitalWrite(ANALOG_MUX_S0, HIGH);
  } else {
    digitalWrite(ANALOG_MUX_S0, LOW);
  }
  
  if (pin & 0x2) {
    digitalWrite(ANALOG_MUX_S1, HIGH);
  } else {
    digitalWrite(ANALOG_MUX_S1, LOW);
  }
  
  if (pin & 0x4) {
    digitalWrite(ANALOG_MUX_S2, HIGH);
  } else {
    digitalWrite(ANALOG_MUX_S2, LOW);
  }
  
  if (pin & 0x8) {
    digitalWrite(ANALOG_MUX_S3, HIGH);
  } else {
    digitalWrite(ANALOG_MUX_S3, LOW);
  }
}
