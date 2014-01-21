/*******************************************************************************
AeroC2
Copyright (C) 2014  Jacob McGladdery

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef AEROC2_TYPES_H_
#define AEROC2_TYPES_H_

#include <stdint.h>

#define AEROC2_STX                (0x55)
#define AEROC2_STX_LEN            (1)
#define AEROC2_CHK_LEN            (4)
#define AEROC2_HEADER_LEN         (5)
#define AEROC2_FOOTER_LEN         (AEROC2_CHK_LEN)
#define AEROC2_HEADER_STX_LEN     (AEROC2_HEADER_LEN + AEROC2_STX_LEN)
#define AEROC2_HEADER_FOOTER_LEN  (AEROC2_HEADER_LEN + AEROC2_FOOTER_LEN)
#define AEROC2_FULL_LEN           (AEROC2_HEADER_STX_LEN + AEROC2_FOOTER_LEN)

#define AEROC2_MAX_PAYLOAD_LEN    (255)
#define AEROC2_MIN_MESSAGE_LEN    (AEROC2_FULL_LEN)
#define AEROC2_MAX_MESSAGE_LEN  \
  (AEROC2_MIN_MESSAGE_LEN + AEROC2_MAX_PAYLOAD_LEN)


/**
 * Action commands to be communicated between the aircraft and the ground
 * station.
 */
enum AEROC2_ACTION
{
  AEROC2_ACTION_HOLD,                /**< Hold position. */
  AEROC2_ACTION_LOITER,              /**< Loiter at a position. */
  AEROC2_ACTION_MOTORS_START,        /**< Allow the motors to start. */
  AEROC2_ACTION_TAKEOFF,             /**< Perform takeoff. */
  AEROC2_ACTION_LAND,                /**< Perform landing. */
  AEROC2_ACTION_EMERGENCY_LAND,      /**< Perform landing immediately. */
  AEROC2_ACTION_EMERGENCY_KILL,      /**< All motors off immediately. */
  AEROC2_ACTION_CONFIRM_KILL,        /**< Confirm all motors are off. */
  AEROC2_ACTION_RETURN_HOME,         /**< Return to the home position. */
  AEROC2_ACTION_CONTINUE,            /**< Do no new action. */
  AEROC2_ACTION_HALT,                /**< Cancel an action. */
  AEROC2_ACTION_SHUTDOWN,            /**< Tell the system to power off. */
  AEROC2_ACTION_REBOOT,              /**< Tell the system to reboot. */
  AEROC2_ACTION_SET_MANUAL,          /**< Set the system to RC control only. */
  AEROC2_ACTION_SET_AUTO,            /**< Set the system to run autonomously. */
  AEROC2_ACTION_STORAGE_READ,        /**< Read from secondary storage. */
  AEROC2_ACTION_STORAGE_WRITE,       /**< Write to secondary storage. */
  AEROC2_ACTION_CALIBRATE_RC,        /**< Calibrate RC parameters. */
  AEROC2_ACTION_CALIBRATE_GYRO,      /**< Calibrate the gyroscope. */
  AEROC2_ACTION_CALIBRATE_ACCEL,     /**< Calibrate the accelerometer. */
  AEROC2_ACTION_CALIBRATE_MAG,       /**< Calibrate the magnetometer. */
  AEROC2_ACTION_CALIBRATE_PRESSURE,  /**< Calibrate air pressure measurement. */
  AEROC2_ACTION_REC_START,           /**< Start recording video. */
  AEROC2_ACTION_REC_PAUSE,           /**< Pause recording video. */
  AEROC2_ACTION_REC_STOP,            /**< Stop recording video. */
  AEROC2_ACTION_NAVIGATE,            /**< Navigate to a waypoint. */
  AEROC2_ACTION_MOTORS_STOP,         /**< Allow the motors to stop. */
  AEROC2_ACTION_NUM                  /**< The number of known actions. */
};


/**
 * The status states of the aircraft system itself.
 */
enum AEROC2_SYS
{
  AEROC2_SYS_START,        /**< Start state. */
  AEROC2_SYS_BOOT,         /**< System is booting up. */
  AEROC2_SYS_CALIBRATING,  /**< System is calibrating devices. */
  AEROC2_SYS_STANDBY,      /**< Ready and standing by. */
  AEROC2_SYS_ACTIVE,       /**< Executing protocol. */
  AEROC2_SYS_CRITICAL,     /**< Encountered a severe error. */
  AEROC2_SYS_EMERGENCY,    /**< Dangerous situation imminent. */
  AEROC2_SYS_SHUTDOWN      /**< System powering off. */
};


/**
 * The status states related to navigation.
 */
enum AEROC2_NAV
{
  AEROC2_NAV_GROUNDED,     /**< Vehicle is on the ground. */
  AEROC2_NAV_TAKEOFF,      /**< Takeoff in progress. */
  AEROC2_NAV_HOLD,         /**< Holding position. */
  AEROC2_NAV_AT_WAYPOINT,  /**< Arrived at waypoint. */
  AEROC2_NAV_ON_APPROACH,  /**< On approach to next waypoint. */
  AEROC2_NAV_RETURN_HOME,  /**< On approach to the home waypoint. */
  AEROC2_NAV_LANDING,      /**< Landing in progress. */
  AEROC2_NAV_LOST          /**< Approach vector or position is incorrect. */
};


/**
 * Core aircraft components.
 */
enum AEROC2_COMPONENT
{
  AEROC2_COMP_ID_NONE,
  AEROC2_COMP_ID_GPS,
  AEROC2_COMP_ID_IMU,
  AEROC2_COMP_ID_ALTIMETER,
  AEROC2_COMP_ID_AIRSPEED,
  AEROC2_COMP_ID_COMPASS,
  AEROC2_COMP_ID_FLIGHT_PLANNER
};


/**
 * Different types of vehicle.
 */
enum AEROC2_VEHICLE
{
  AEROC2_GENERIC,
  AEROC2_FIXED_WING,
  AEROC2_QUADROTOR,
  AEROC2_COAXIAL,
  AEROC2_HELICOPTER,
  AEROC2_GROUND
};


/**
 * Control modes.
 */
enum AEROC2_MODE
{
  AEROC2_MANUAL,
  AEROC2_AUTONOMOUS
};


/**
 * Parse states.
 */
typedef enum
{
  AEROC2_PARSE_STATE_START = 0,
  AEROC2_PARSE_STATE_IDLE,
  AEROC2_PARSE_STATE_GOT_STX,
  AEROC2_PARSE_STATE_GOT_LEN,
  AEROC2_PARSE_STATE_GOT_SEQ,
  AEROC2_PARSE_STATE_GOT_SYS,
  AEROC2_PARSE_STATE_GOT_COMP,
  AEROC2_PARSE_STATE_GOT_TYPE,
  AEROC2_PARSE_STATE_GOT_PAYLOAD,
  AEROC2_PARSE_STATE_NUM
} aeroc2_parse_state_t;


/**
 * Generic 16-bit type.
 */
typedef union
{
  uint8_t  c[2];
  uint16_t s;
  int16_t  i;
} __generic16_t;


/**
 * Generic 32-bit type.
 */
typedef union
{
  uint8_t  c[4];
  uint32_t s;
  int32_t  i;
  float    f;
} __generic32_t;


/**
 * Generic 64-bit type.
 */
typedef union
{
  uint8_t  c[8];
  uint64_t s;
  int64_t  i;
} __generic64_t;


/**
 * Message structure.
 */
typedef struct
{
  uint8_t  len;                              /**< Payload length. */
  uint8_t  seq;                              /**< Sequence number. */
  uint8_t  sys;                              /**< Source system ID. */
  uint8_t  comp;                             /**< Source component ID. */
  uint8_t  type;                             /**< The type of payload. */
  uint8_t  payload[AEROC2_MAX_PAYLOAD_LEN];  /**< The actual message. */
  uint32_t chk;                              /**< CRC-32Q checksum. */
} aeroc2_message_t;


/**
 * Status structure.
 */
typedef struct
{
  uint32_t chk;                /**< CRC-32Q checksum. */
  uint8_t  chk_index;          /**< Current checksum index. */
  uint8_t  current_seq;        /**< Current sequence number. */
  uint8_t  msg_received;       /**< Boolean for full message received */
  uint8_t  num_parse_error;    /**< Number of parsing errors. */
  uint64_t num_rx_drop;        /**< Number of received messages droppped. */
  uint64_t num_rx_success;     /**< Number of usable received messages. */
  uint8_t  payload_index;      /**< Current payload index. */
  aeroc2_parse_state_t state;  /**< Current parse state. */
} aeroc2_status_t;

#endif  /* AEROC2_TYPES_H_ */
