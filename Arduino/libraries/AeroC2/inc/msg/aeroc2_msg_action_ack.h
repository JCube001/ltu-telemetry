#ifndef AEROC2_MSG_ACTION_ACK_H_
#define AEROC2_MSG_ACTION_ACK_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Action acknowledgment data.
 */
typedef struct
{
  uint8_t action;  /**< The action ID. */
  uint8_t result;  /**< Boolean for whether the action was denied or allowed. */
} aeroc2_action_ack_t;


/**
 * Pack an action acknowledgment message.
 *
 * @param[out] msg       The message to pack.
 * @param[in]  sys_id    The system ID.
 * @param[in]  comp_id   The component ID.
 * @param[in]  action    The action ID.
 * @param[in]  result    A 0 for a denied action, a 1 for an allowed action.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_action_ack_pack(aeroc2_message_t* msg,
                           const uint8_t sys_id,
                           const uint8_t comp_id,
                           const uint8_t action,
                           const uint8_t result)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_ACTION_ACK;
  
  i += put_uint8_at_index(msg->payload, action, i);
  i += put_uint8_at_index(msg->payload, result, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode an action acknowledgment message.
 *
 * @note Identical to aeroc2_msg_action_ack_pack().
 *
 * @param[out] msg       The message to pack.
 * @param[in]  sys_id    The system ID.
 * @param[in]  comp_id   The component ID.
 * @param[in]  target    The target system.
 * @param[in]  component The component executing the action.
 * @param[in]  action    The action ID.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_action_ack_encode(aeroc2_message_t* msg,
                             const uint8_t sys_id,
                             const uint8_t comp_id,
                             const uint8_t action,
                             const uint8_t result)
{
  return aeroc2_msg_action_ack_pack(msg, sys_id, comp_id, action, result);
}


/**
 * Get the action field from the message.
 *
 * @param[in] msg The message.
 * @return The action ID.
 */
static inline uint8_t
aeroc2_msg_action_ack_get_action(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload)[0];
}


/**
 * Get the result field from the message.
 *
 * @param[in] msg The message.
 * @return The result boolean.
 */
static inline uint8_t
aeroc2_msg_action_ack_get_result(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload + sizeof(uint8_t))[0];
}


/**
 * Decode the full action acknowledgment message.
 *
 * @param[out] action_ack Storage for the decoded message.
 * @param[in]  msg        The message to decode.
 */
static inline void
aeroc2_msg_action_ack_decode(aeroc2_action_ack_t* action_ack,
                             const aeroc2_message_t* msg)
{
  action_ack->action = aeroc2_msg_action_ack_get_action(msg);
  action_ack->result = aeroc2_msg_action_ack_get_result(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_ACTION_ACK_H_ */
