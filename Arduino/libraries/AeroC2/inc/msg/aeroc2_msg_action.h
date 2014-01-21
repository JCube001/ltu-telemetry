#ifndef AEROC2_MSG_ACTION_H_
#define AEROC2_MSG_ACTION_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Action data.
 */
typedef struct
{
  uint8_t target;     /**< The target system. */
  uint8_t component;  /**< The component executing the action. */
  uint8_t action;     /**< The action ID. */
} aeroc2_action_t;


/**
 * Pack an action message.
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
aeroc2_msg_action_pack(aeroc2_message_t* msg,
                       const uint8_t sys_id,
                       const uint8_t comp_id,
                       const uint8_t target,
                       const uint8_t component,
                       const uint8_t action)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_ACTION;
  
  i += put_uint8_at_index(msg->payload, target, i);
  i += put_uint8_at_index(msg->payload, component, i);
  i += put_uint8_at_index(msg->payload, action, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode an action message.
 *
 * @note Identical to aeroc2_msg_action_pack().
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
aeroc2_msg_action_encode(aeroc2_message_t* msg,
                         const uint8_t sys_id,
                         const uint8_t comp_id,
                         const uint8_t target,
                         const uint8_t component,
                         const uint8_t action)
{
  return aeroc2_msg_action_pack(msg, sys_id, comp_id,
                                target, component, action);
}


/**
 * Get the target field from the message.
 *
 * @param[in] msg The message.
 * @return The target system ID.
 */
static inline uint8_t
aeroc2_msg_action_get_target(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload)[0];
}


/**
 * Get the component field from the message.
 *
 * @param[in] msg The message.
 * @return The target component ID.
 */
static inline uint8_t
aeroc2_msg_action_get_component(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload + sizeof(uint8_t))[0];
}


/**
 * Get the action field from the message.
 *
 * @param[in] msg The message.
 * @return The action ID.
 */
static inline uint8_t
aeroc2_msg_action_get_action(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload + (2 * sizeof(uint8_t)))[0];
}


/**
 * Decode the full action message.
 *
 * @param[out] action Storage for the decoded message.
 * @param[in]  msg    The message to decode.
 */
static inline void
aeroc2_msg_action_decode(aeroc2_action_t* action,
                         const aeroc2_message_t* msg)
{
  action->target = aeroc2_msg_action_get_target(msg);
  action->component = aeroc2_msg_action_get_component(msg);
  action->action = aeroc2_msg_action_get_action(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_ACTION_H_ */
