#ifndef AEROC2_MSG_PRESSURE_H_
#define AEROC2_MSG_PRESSURE_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Pressure data.
 */
typedef struct
{
  float pressure;     /**< Pascal (Pa). */
  float temperature;  /**< Kelvin (K). */
} aeroc2_pressure_t;


/**
 * Pack a pressure message.
 *
 * @param[out] msg         The message to pack.
 * @param[in]  sys_id      The system ID.
 * @param[in]  comp_id     The component ID.
 * @param[in]  pressure    Pressure in pascals (Pa).
 * @param[in]  temperature Temperature in kelvins (K).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_pressure_pack(aeroc2_message_t* msg,
                         const uint8_t sys_id,
                         const uint8_t comp_id,
                         const float pressure,
                         const float temperature)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_PRESSURE;
  
  i += put_float_at_index(msg->payload, pressure, i);
  i += put_float_at_index(msg->payload, temperature, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a pressure message.
 *
 * @note Identical to aeroc2_msg_pressure_pack().
 *
 * @param[out] msg         The message to pack.
 * @param[in]  sys_id      The system ID.
 * @param[in]  comp_id     The component ID.
 * @param[in]  pressure    Pressure in pascals (Pa).
 * @param[in]  temperature Temperature in kelvins (K).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_pressure_encode(aeroc2_message_t* msg,
                           const uint8_t sys_id,
                           const uint8_t comp_id,
                           const float pressure,
                           const float temperature)
{
  return aeroc2_msg_pressure_pack(msg, sys_id, comp_id, pressure, temperature);
}


/**
 * Get the pressure field from the message.
 *
 * @param[in] msg The message.
 * @return The pressure in pascals (Pa).
 */
static inline float
aeroc2_msg_pressure_get_pressure(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (float)g.f;
}


/**
 * Get the temperature field from the message.
 *
 * @param[in] msg The message.
 * @return The temperature in kelvins (K).
 */
static inline float
aeroc2_msg_pressure_get_temperature(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + sizeof(float))[0];
  g.c[2] = (msg->payload + sizeof(float))[1];
  g.c[1] = (msg->payload + sizeof(float))[2];
  g.c[0] = (msg->payload + sizeof(float))[3];
  return (float)g.f;
}


/**
 * Decode the full pressure message.
 *
 * @param[out] pressure Storage for the decoded message.
 * @param[in]  msg      The message to decode.
 */
static inline void
aeroc2_msg_pressure_decode(aeroc2_pressure_t* pressure,
                           const aeroc2_message_t* msg)
{
  pressure->pressure = aeroc2_msg_pressure_get_pressure(msg);
  pressure->temperature = aeroc2_msg_pressure_get_temperature(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_PRESSURE_H_ */
