#ifndef AEROC2_MSG_ATTITUDE_H_
#define AEROC2_MSG_ATTITUDE_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Attitude data.
 */
typedef struct
{
  float yaw;          /**< Rotation about the z-axis (rad). */
  float pitch;        /**< Rotation about the x-axis (rad). */
  float roll;         /**< Rotation about the y-axis (rad). */
  float yaw_speed;    /**< Yaw angular speed (rad/s). */
  float pitch_speed;  /**< Pitch angular speed (rad/s). */
  float roll_speed;   /**< Roll angular speed (rad/s). */
} aeroc2_attitude_t;


/**
 * Pack an attitude message.
 *
 * @param[out] msg          The message to pack.
 * @param[in]  sys_id       The system ID.
 * @param[in]  comp_id      The component ID.
 * @param[in]  yaw          Rotation about the z-axis (rad).
 * @param[in]  pitch        Rotation about the x-axis (rad).
 * @param[in]  roll         Rotation about the y-axis (rad).
 * @param[in]  yaw_speed    Yaw angular speed (rad/s).
 * @param[in]  pitch_speed  Pitch angular speed (rad/s).
 * @param[in]  roll_speed   Roll angular speed (rad/s).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_attitude_pack(aeroc2_message_t* msg,
                         const uint8_t sys_id,
                         const uint8_t comp_id,
                         const float yaw,
                         const float pitch,
                         const float roll,
                         const float yaw_speed,
                         const float pitch_speed,
                         const float roll_speed)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_ATTITUDE;
  
  i += put_float_at_index(msg->payload, yaw, i);
  i += put_float_at_index(msg->payload, pitch, i);
  i += put_float_at_index(msg->payload, roll, i);
  i += put_float_at_index(msg->payload, yaw_speed, i);
  i += put_float_at_index(msg->payload, pitch_speed, i);
  i += put_float_at_index(msg->payload, roll_speed, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode an attitude message.
 *
 * @note Identical to aeroc2_msg_attitude_pack().
 *
 * @param[out] msg          The message to pack.
 * @param[in]  sys_id       The system ID.
 * @param[in]  comp_id      The component ID.
 * @param[in]  yaw          Rotation about the z-axis (rad).
 * @param[in]  pitch        Rotation about the x-axis (rad).
 * @param[in]  roll         Rotation about the y-axis (rad).
 * @param[in]  yaw_speed    Yaw angular speed (rad/s).
 * @param[in]  pitch_speed  Pitch angular speed (rad/s).
 * @param[in]  roll_speed   Roll angular speed (rad/s).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_attitude_encode(aeroc2_message_t* msg,
                           const uint8_t sys_id,
                           const uint8_t comp_id,
                           const float yaw,
                           const float pitch,
                           const float roll,
                           const float yaw_speed,
                           const float pitch_speed,
                           const float roll_speed)
{
  return aeroc2_msg_attitude_pack(msg, sys_id, comp_id,
                                  yaw, pitch, roll, yaw_speed, pitch_speed,
                                  roll_speed);
}


/**
 * Get the yaw field from the message.
 *
 * @param[in] msg The message.
 * @return Rotation about the z-axis (rad).
 */
static inline float
aeroc2_msg_attitude_get_yaw(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (float)g.f;
}


/**
 * Get the pitch field from the message.
 *
 * @param[in] msg The message.
 * @return Rotation about the x-axis (rad).
 */
static inline float
aeroc2_msg_attitude_get_pitch(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + sizeof(float))[0];
  g.c[2] = (msg->payload + sizeof(float))[1];
  g.c[1] = (msg->payload + sizeof(float))[2];
  g.c[0] = (msg->payload + sizeof(float))[3];
  return (float)g.f;
}


/**
 * Get the roll field from the message.
 *
 * @param[in] msg The message.
 * @return Rotation about the y-axis (rad).
 */
static inline float
aeroc2_msg_attitude_get_roll(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the yaw speed field from the message.
 *
 * @param[in] msg The message.
 * @return Yaw angular speed (rad/s).
 */
static inline float
aeroc2_msg_attitude_get_yaw_speed(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (3 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (3 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (3 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (3 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the pitch speed field from the message.
 *
 * @param[in] msg The message.
 * @return Pitch angular speed (rad/s).
 */
static inline float
aeroc2_msg_attitude_get_pitch_speed(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (4 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (4 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (4 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (4 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the roll speed field from the message.
 *
 * @param[in] msg The message.
 * @return Roll angular speed (rad/s).
 */
static inline float
aeroc2_msg_attitude_get_roll_speed(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (5 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (5 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (5 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (5 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Decode the full attitude message.
 *
 * @param[out] attitude Storage for the decoded message.
 * @param[in]  msg      The message to decode.
 */
static inline void
aeroc2_msg_attitude_decode(aeroc2_attitude_t* attitude,
                           const aeroc2_message_t* msg)
{
  attitude->yaw = aeroc2_msg_attitude_get_yaw(msg);
  attitude->pitch = aeroc2_msg_attitude_get_pitch(msg);
  attitude->roll = aeroc2_msg_attitude_get_roll(msg);
  attitude->yaw_speed = aeroc2_msg_attitude_get_yaw_speed(msg);
  attitude->pitch_speed = aeroc2_msg_attitude_get_pitch_speed(msg);
  attitude->roll_speed = aeroc2_msg_attitude_get_roll_speed(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_ATTITUDE_H_ */
