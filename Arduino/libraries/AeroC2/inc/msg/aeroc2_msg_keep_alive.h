#ifndef AEROC2_MSG_KEEP_ALIVE_H_
#define AEROC2_MSG_KEEP_ALIVE_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Keep alive data.
 */
typedef struct
{
  uint8_t vehicle;  /**< The type of vehicle. */
  uint8_t mode;     /**< The autopilot mode of the vehicle. */
} aeroc2_keep_alive_t;


/**
 * Pack a keep alive message.
 *
 * @param[out] msg     The message to pack.
 * @param[in]  sys_id  The system ID.
 * @param[in]  comp_id The component ID.
 * @param[in]  vehicle The type of vehicle.
 * @param[in]  mode    The autopilot mode of the vehicle.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_keep_alive_pack(aeroc2_message_t* msg,
                           const uint8_t sys_id,
                           const uint8_t comp_id,
                           const uint8_t vehicle,
                           const uint8_t mode)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_KEEP_ALIVE;
  
  i += put_uint8_at_index(msg->payload, vehicle, i);
  i += put_uint8_at_index(msg->payload, mode, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a keep alive message.
 *
 * @note Identical to aeroc2_msg_keep_alive_pack().
 *
 * @param[out] msg     The message to pack.
 * @param[in]  sys_id  The system ID.
 * @param[in]  comp_id The component ID.
 * @param[in]  vehicle The type of vehicle.
 * @param[in]  mode    The autopilot mode of the vehicle.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_keep_alive_encode(aeroc2_message_t* msg,
                             const uint8_t sys_id,
                             const uint8_t comp_id,
                             const uint8_t vehicle,
                             const uint8_t mode)
{
  return aeroc2_msg_keep_alive_pack(msg, sys_id, comp_id, vehicle, mode);
}


/**
 * Get the vehicle field from the message.
 *
 * @param[in] msg The message.
 * @return The vehicle type.
 */
static inline uint8_t
aeroc2_msg_keep_alive_get_vehicle(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload)[0];
}


/**
 * Get the mode field from the message.
 *
 * @param[in] msg The message.
 * @return The autopilot mode.
 */
static inline uint8_t
aeroc2_msg_keep_alive_get_mode(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload + sizeof(uint8_t))[0];
}


/**
 * Decode the full keep alive message.
 *
 * @param[out] keep_alive Storage for the decoded message.
 * @param[in]  msg        The message to decode.
 */
static inline void
aeroc2_msg_keep_alive_decode(aeroc2_keep_alive_t* keep_alive,
                             const aeroc2_message_t* msg)
{
  keep_alive->vehicle = aeroc2_msg_keep_alive_get_vehicle(msg);
  keep_alive->mode = aeroc2_msg_keep_alive_get_mode(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_KEEP_ALIVE_H_ */
