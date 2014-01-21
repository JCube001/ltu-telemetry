#ifndef AEROC2_MSG_H_
#define AEROC2_MSG_H_

/**
 * All SI units are from http://physics.nist.gov/cuu/Units/units.html
 */

/* Must be included before any others */
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

/* All of the different messages */
#include "aeroc2_msg_action_ack.h"
#include "aeroc2_msg_action.h"
#include "aeroc2_msg_airspeed.h"
#include "aeroc2_msg_altitude.h"
#include "aeroc2_msg_attitude.h"
#include "aeroc2_msg_attitude_quaternion.h"
#include "aeroc2_msg_boot.h"
#include "aeroc2_msg_debug.h"
#include "aeroc2_msg_gps.h"
#include "aeroc2_msg_gps_position.h"
#include "aeroc2_msg_gps_raw.h"
#include "aeroc2_msg_gps_status.h"
#include "aeroc2_msg_imu.h"
#include "aeroc2_msg_imu_raw.h"
#include "aeroc2_msg_keep_alive.h"
#include "aeroc2_msg_pressure.h"
#include "aeroc2_msg_status_text.h"

#endif  /* AEROC2_MSG_H_ */
