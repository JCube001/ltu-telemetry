#ifndef AEROC2_MSG_IMU_H_
#define AEROC2_MSG_IMU_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * IMU data.
 */
typedef struct
{
  float acc_x;   /**< X-axis acceleration (m/s^2) */
  float acc_y;   /**< Y-axis acceleration (m/s^2) */
  float acc_z;   /**< Z-axis acceleration (m/s^2) */
  float gyro_x;  /**< X-axis angular velocity (rad/s) */
  float gyro_y;  /**< Y-axis angular velocity (rad/s) */
  float gyro_z;  /**< Z-axis angular velocity (rad/s) */
  float mag_x;   /**< X-axis magnetic flux density (T) */
  float mag_y;   /**< Y-axis magnetic flux density (T) */
  float mag_z;   /**< Z-axis magnetic flux density (T) */
} aeroc2_imu_t;


/**
 * Pack an IMU message.
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  acc_x    X-axis acceleration (m/s^2).
 * @param[in]  acc_y    Y-axis acceleration (m/s^2).
 * @param[in]  acc_z    Z-axis acceleration (m/s^2).
 * @param[in]  gyro_x   X-axis angular velocity (rad/s).
 * @param[in]  gyro_y   Y-axis angular velocity (rad/s).
 * @param[in]  gyro_z   Z-axis angular velocity (rad/s).
 * @param[in]  mag_x    X-axis magnetic flux density (T).
 * @param[in]  mag_y    Y-axis magnetic flux density (T).
 * @param[in]  mag_z    Z-axis magnetic flux density (T).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_imu_pack(aeroc2_message_t* msg,
                    const uint8_t sys_id,
                    const uint8_t comp_id,
                    const float acc_x,
                    const float acc_y,
                    const float acc_z,
                    const float gyro_x,
                    const float gyro_y,
                    const float gyro_z,
                    const float mag_x,
                    const float mag_y,
                    const float mag_z)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_IMU;
  
  i += put_float_at_index(msg->payload, acc_x, i);
  i += put_float_at_index(msg->payload, acc_y, i);
  i += put_float_at_index(msg->payload, acc_z, i);
  i += put_float_at_index(msg->payload, gyro_x, i);
  i += put_float_at_index(msg->payload, gyro_y, i);
  i += put_float_at_index(msg->payload, gyro_z, i);
  i += put_float_at_index(msg->payload, mag_x, i);
  i += put_float_at_index(msg->payload, mag_y, i);
  i += put_float_at_index(msg->payload, mag_z, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode an IMU message.
 *
 * @note Identical to aeroc2_msg_imu_pack().
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  acc_x    X-axis acceleration (m/s^2).
 * @param[in]  acc_y    Y-axis acceleration (m/s^2).
 * @param[in]  acc_z    Z-axis acceleration (m/s^2).
 * @param[in]  gyro_x   X-axis angular velocity (rad/s).
 * @param[in]  gyro_y   Y-axis angular velocity (rad/s).
 * @param[in]  gyro_z   Z-axis angular velocity (rad/s).
 * @param[in]  mag_x    X-axis magnetic flux density (T).
 * @param[in]  mag_y    Y-axis magnetic flux density (T).
 * @param[in]  mag_z    Z-axis magnetic flux density (T).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_imu_encode(aeroc2_message_t* msg,
                      const uint8_t sys_id,
                      const uint8_t comp_id,
                      const float acc_x,
                      const float acc_y,
                      const float acc_z,
                      const float gyro_x,
                      const float gyro_y,
                      const float gyro_z,
                      const float mag_x,
                      const float mag_y,
                      const float mag_z)
{
  return aeroc2_msg_imu_pack(msg, sys_id, comp_id,
                             acc_x, acc_y, acc_z,
                             gyro_x, gyro_y, gyro_z,
                             mag_x, mag_y, mag_z);
}


/**
 * Get the acc_x field from the message.
 *
 * @param[in] msg The message.
 * @return The x-axis acceleration in meters per second squared (m/s^2).
 */
static inline float
aeroc2_msg_imu_get_acc_x(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (float)g.f;
}


/**
 * Get the acc_y field from the message.
 *
 * @param[in] msg The message.
 * @return The y-axis acceleration in meters per second squared (m/s^2).
 */
static inline float
aeroc2_msg_imu_get_acc_y(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + sizeof(float))[0];
  g.c[2] = (msg->payload + sizeof(float))[1];
  g.c[1] = (msg->payload + sizeof(float))[2];
  g.c[0] = (msg->payload + sizeof(float))[3];
  return (float)g.f;
}


/**
 * Get the acc_z field from the message.
 *
 * @param[in] msg The message.
 * @return The z-axis acceleration in meters per second squared (m/s^2).
 */
static inline float
aeroc2_msg_imu_get_acc_z(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the gyro_x field from the message.
 *
 * @param[in] msg The message.
 * @return The x-axis angular velocity in radians per second (rad/s).
 */
static inline float
aeroc2_msg_imu_get_gyro_x(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (3 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (3 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (3 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (3 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the gyro_y field from the message.
 *
 * @param[in] msg The message.
 * @return The y-axis angular velocity in radians per second (rad/s).
 */
static inline float
aeroc2_msg_imu_get_gyro_y(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (4 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (4 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (4 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (4 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the gyro_z field from the message.
 *
 * @param[in] msg The message.
 * @return The z-axis angular velocity in radians per second (rad/s).
 */
static inline float
aeroc2_msg_imu_get_gyro_z(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (5 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (5 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (5 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (5 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the mag_x field from the message.
 *
 * @param[in] msg The message.
 * @return The x-axis magnetic flux density in teslas (T).
 */
static inline float
aeroc2_msg_imu_get_mag_x(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (6 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (6 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (6 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (6 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the mag_y field from the message.
 *
 * @param[in] msg The message.
 * @return The y-axis magnetic flux density in teslas (T).
 */
static inline float
aeroc2_msg_imu_get_mag_y(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (7 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (7 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (7 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (7 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the mag_z field from the message.
 *
 * @param[in] msg The message.
 * @return The z-axis magnetic flux density in teslas (T).
 */
static inline float
aeroc2_msg_imu_get_mag_z(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (8 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (8 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (8 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (8 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Decode the full IMU message.
 *
 * @param[out] imu Storage for the decoded message.
 * @param[in]  msg The message to decode.
 */
static inline void
aeroc2_msg_imu_decode(aeroc2_imu_t* imu,
                      const aeroc2_message_t* msg)
{
  imu->acc_x = aeroc2_msg_imu_get_acc_x(msg);
  imu->acc_y = aeroc2_msg_imu_get_acc_y(msg);
  imu->acc_z = aeroc2_msg_imu_get_acc_z(msg);
  imu->gyro_x = aeroc2_msg_imu_get_gyro_x(msg);
  imu->gyro_y = aeroc2_msg_imu_get_gyro_y(msg);
  imu->gyro_z = aeroc2_msg_imu_get_gyro_z(msg);
  imu->mag_x = aeroc2_msg_imu_get_mag_x(msg);
  imu->mag_y = aeroc2_msg_imu_get_mag_y(msg);
  imu->mag_z = aeroc2_msg_imu_get_mag_z(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_IMU_H_ */
