#ifndef AEROC2_MSG_AIRSPEED_H_
#define AEROC2_MSG_AIRSPEED_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Airspeed data.
 */
typedef struct
{
  float airspeed;  /**< Meters per second (m/s). */
} aeroc2_airspeed_t;


/**
 * Pack an airspeed message.
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  airspeed Airspeed in meters per second (m/s).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_airspeed_pack(aeroc2_message_t* msg,
                         const uint8_t sys_id,
                         const uint8_t comp_id,
                         const float airspeed)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_AIRSPEED;
  
  i += put_float_at_index(msg->payload, airspeed, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode an airspeed message.
 *
 * @note Identical to aeroc2_msg_airspeed_pack().
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  airspeed Airspeed in meters per second (m/s).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_airspeed_encode(aeroc2_message_t* msg,
                           const uint8_t sys_id,
                           const uint8_t comp_id,
                           const float airspeed)
{
  return aeroc2_msg_airspeed_pack(msg, sys_id, comp_id, airspeed);
}


/**
 * Get the airspeed field from the message.
 *
 * @param[in] msg The message.
 * @return The airspeed in meters per second (m/s).
 */
static inline float
aeroc2_msg_airspeed_get_airspeed(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (float)g.f;
}


/**
 * Decode the full airspeed message.
 *
 * @param[out] airspeed Storage for the decoded message.
 * @param[in]  msg      The message to decode.
 */
static inline void
aeroc2_msg_airspeed_decode(aeroc2_airspeed_t* airspeed,
                           const aeroc2_message_t* msg)
{
  airspeed->airspeed = aeroc2_msg_airspeed_get_airspeed(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_AIRSPEED_H_ */
