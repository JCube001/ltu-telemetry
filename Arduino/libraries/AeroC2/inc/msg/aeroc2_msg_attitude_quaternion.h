#ifndef AEROC2_MSG_ATTITUDE_QUATERNION_H_
#define AEROC2_MSG_ATTITUDE_QUATERNION_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Attitude quaternion data.
 */
typedef struct
{
  float w;  /**< Real angular component. */
  float x;  /**< Imaginary x-axis vectorial component. */
  float y;  /**< Imaginary y-axis vectorial component. */
  float z;  /**< Imaginary z-axis vectorial component. */
} aeroc2_attitude_quaternion_t;


/**
 * Pack an attitude quaternion message.
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  w        Real angular component.
 * @param[in]  x        Imaginary x-axis vectorial component.
 * @param[in]  y        Imaginary y-axis vectorial component.
 * @param[in]  z        Imaginary z-axis vectorial component.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_attitude_quaternion_pack(aeroc2_message_t* msg,
                                    const uint8_t sys_id,
                                    const uint8_t comp_id,
                                    const float w,
                                    const float x,
                                    const float y,
                                    const float z)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_ATTITUDE_QUATERNION;
  
  i += put_float_at_index(msg->payload, w, i);
  i += put_float_at_index(msg->payload, x, i);
  i += put_float_at_index(msg->payload, y, i);
  i += put_float_at_index(msg->payload, z, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode an attitude quaternion message.
 *
 * @note Identical to aeroc2_msg_attitude_quaternion_pack().
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  w        Real angular component.
 * @param[in]  x        Imaginary x-axis vectorial component.
 * @param[in]  y        Imaginary y-axis vectorial component.
 * @param[in]  z        Imaginary z-axis vectorial component.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_attitude_quaternion_encode(aeroc2_message_t* msg,
                                      const uint8_t sys_id,
                                      const uint8_t comp_id,
                                      const float w,
                                      const float x,
                                      const float y,
                                      const float z)
{
  return aeroc2_msg_attitude_quaternion_pack(msg, sys_id, comp_id, w, x, y, z);
}


/**
 * Get the w field from the message.
 *
 * @param[in] msg The message.
 * @return Real angular component.
 */
static inline float
aeroc2_msg_attitude_quaternion_get_w(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (float)g.f;
}


/**
 * Get the x field from the message.
 *
 * @param[in] msg The message.
 * @return Imaginary x-axis vectorial component.
 */
static inline float
aeroc2_msg_attitude_quaternion_get_x(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + sizeof(float))[0];
  g.c[2] = (msg->payload + sizeof(float))[1];
  g.c[1] = (msg->payload + sizeof(float))[2];
  g.c[0] = (msg->payload + sizeof(float))[3];
  return (float)g.f;
}


/**
 * Get the y field from the message.
 *
 * @param[in] msg The message.
 * @return Imaginary y-axis vectorial component.
 */
static inline float
aeroc2_msg_attitude_quaternion_get_y(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the z field from the message.
 *
 * @param[in] msg The message.
 * @return Imaginary z-axis vectorial component.
 */
static inline float
aeroc2_msg_attitude_quaternion_get_z(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (3 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (3 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (3 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (3 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Decode the full attitude quaternion message.
 *
 * @param[out] attitude Storage for the decoded message.
 * @param[in]  msg      The message to decode.
 */
static inline void
aeroc2_msg_attitude_quaternion_decode(aeroc2_attitude_quaternion_t* attitude,
                                      const aeroc2_message_t* msg)
{
  attitude->w = aeroc2_msg_attitude_quaternion_get_w(msg);
  attitude->x = aeroc2_msg_attitude_quaternion_get_x(msg);
  attitude->y = aeroc2_msg_attitude_quaternion_get_y(msg);
  attitude->z = aeroc2_msg_attitude_quaternion_get_z(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_ATTITUDE_QUATERNION_H_ */
