#ifndef AEROC2_MSG_ALTITUDE_H_
#define AEROC2_MSG_ALTITUDE_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Altitude data.
 */
typedef struct
{
  float altitude;  /**< Meters (m). */
} aeroc2_altitude_t;


/**
 * Pack an altitude message.
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  altitude Altitude in meters (m).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_altitude_pack(aeroc2_message_t* msg,
                         const uint8_t sys_id,
                         const uint8_t comp_id,
                         const float altitude)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_ALTITUDE;
  
  i += put_float_at_index(msg->payload, altitude, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode an altitude message.
 *
 * @note Identical to aeroc2_msg_altitude_pack().
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  altitude Altitude in meters (m).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_altitude_encode(aeroc2_message_t* msg,
                           const uint8_t sys_id,
                           const uint8_t comp_id,
                           const float altitude)
{
  return aeroc2_msg_altitude_pack(msg, sys_id, comp_id, altitude);
}


/**
 * Get the altitude field from the message.
 *
 * @param[in] msg The message.
 * @return The altitude in meters (m).
 */
static inline float
aeroc2_msg_altitude_get_altitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (float)g.f;
}


/**
 * Decode the full altitude message.
 *
 * @param[out] altitude Storage for the decoded message.
 * @param[in]  msg      The message to decode.
 */
static inline void
aeroc2_msg_altitude_decode(aeroc2_altitude_t* altitude,
                           const aeroc2_message_t* msg)
{
  altitude->altitude = aeroc2_msg_altitude_get_altitude(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_ALTITUDE_H_ */
