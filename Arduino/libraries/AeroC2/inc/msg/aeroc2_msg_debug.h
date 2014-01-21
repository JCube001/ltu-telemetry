#ifndef AEROC2_MSG_DEBUG_H_
#define AEROC2_MSG_DEBUG_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Debug data.
 */
typedef struct
{
  uint8_t id;   /**< The ID of the debug value. */
  float value;  /**< The debug value. */
} aeroc2_debug_t;


/**
 * Pack a debug message.
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  id       The ID of the debug value.
 * @param[in]  value    The debug value.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_debug_pack(aeroc2_message_t* msg,
                      const uint8_t sys_id,
                      const uint8_t comp_id,
                      const uint8_t id,
                      const float value)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_DEBUG;
  
  i += put_uint8_at_index(msg->payload, id, i);
  i += put_float_at_index(msg->payload, value, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a debug message.
 *
 * @note Identical to aeroc2_msg_debug_pack().
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  id       The ID of the debug value.
 * @param[in]  value    The debug value.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_debug_encode(aeroc2_message_t* msg,
                        const uint8_t sys_id,
                        const uint8_t comp_id,
                        const uint8_t id,
                        const float value)
{
  return aeroc2_msg_debug_pack(msg, sys_id, comp_id, id, value);
}


/**
 * Get the ID field from the message.
 *
 * @param[in] msg The message.
 * @return The ID of the debug value.
 */
static inline uint8_t
aeroc2_msg_debug_get_id(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload)[0];
}


/**
 * Get the value field from the message.
 *
 * @param[in] msg The message.
 * @return The debug value.
 */
static inline float
aeroc2_msg_debug_get_value(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + sizeof(uint8_t))[0];
  g.c[2] = (msg->payload + sizeof(uint8_t))[1];
  g.c[1] = (msg->payload + sizeof(uint8_t))[2];
  g.c[0] = (msg->payload + sizeof(uint8_t))[3];
  return (float)g.f;
}


/**
 * Decode the full debug message.
 *
 * @param[out] debug Storage for the decoded message.
 * @param[in]  msg   The message to decode.
 */
static inline void
aeroc2_msg_debug_decode(aeroc2_debug_t* debug,
                        const aeroc2_message_t* msg)
{
  debug->id = aeroc2_msg_debug_get_id(msg);
  debug->value = aeroc2_msg_debug_get_value(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_DEBUG_H_ */
