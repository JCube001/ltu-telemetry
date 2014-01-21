#ifndef AEROC2_MSG_IMU_RAW_H_
#define AEROC2_MSG_IMU_RAW_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Raw IMU data.
 */
typedef struct
{
  int16_t acc_x;   /**< X-axis acceleration (ADC units) */
  int16_t acc_y;   /**< Y-axis acceleration (ADC units) */
  int16_t acc_z;   /**< Z-axis acceleration (ADC units) */
  int16_t gyro_x;  /**< X-axis angular velocity (ADC units) */
  int16_t gyro_y;  /**< Y-axis angular velocity (ADC units) */
  int16_t gyro_z;  /**< Z-axis angular velocity (ADC units) */
  int16_t mag_x;   /**< X-axis magnetic flux density (ADC units) */
  int16_t mag_y;   /**< Y-axis magnetic flux density (ADC units) */
  int16_t mag_z;   /**< Z-axis magnetic flux density (ADC units) */
} aeroc2_imu_raw_t;


/**
 * Pack a raw IMU message.
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  acc_x    X-axis acceleration (ADC units).
 * @param[in]  acc_y    Y-axis acceleration (ADC units).
 * @param[in]  acc_z    Z-axis acceleration (ADC units).
 * @param[in]  gyro_x   X-axis angular velocity (ADC units).
 * @param[in]  gyro_y   Y-axis angular velocity (ADC units).
 * @param[in]  gyro_z   Z-axis angular velocity (ADC units).
 * @param[in]  mag_x    X-axis magnetic flux density (ADC units).
 * @param[in]  mag_y    Y-axis magnetic flux density (ADC units).
 * @param[in]  mag_z    Z-axis magnetic flux density (ADC units).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_imu_raw_pack(aeroc2_message_t* msg,
                        const uint8_t sys_id,
                        const uint8_t comp_id,
                        const int16_t acc_x,
                        const int16_t acc_y,
                        const int16_t acc_z,
                        const int16_t gyro_x,
                        const int16_t gyro_y,
                        const int16_t gyro_z,
                        const int16_t mag_x,
                        const int16_t mag_y,
                        const int16_t mag_z)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_IMU_RAW;
  
  i += put_int16_at_index(msg->payload, acc_x, i);
  i += put_int16_at_index(msg->payload, acc_y, i);
  i += put_int16_at_index(msg->payload, acc_z, i);
  i += put_int16_at_index(msg->payload, gyro_x, i);
  i += put_int16_at_index(msg->payload, gyro_y, i);
  i += put_int16_at_index(msg->payload, gyro_z, i);
  i += put_int16_at_index(msg->payload, mag_x, i);
  i += put_int16_at_index(msg->payload, mag_y, i);
  i += put_int16_at_index(msg->payload, mag_z, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a raw IMU message.
 *
 * @note Identical to aeroc2_msg_imu_raw_pack().
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
aeroc2_msg_imu_raw_encode(aeroc2_message_t* msg,
                          const uint8_t sys_id,
                          const uint8_t comp_id,
                          const int16_t acc_x,
                          const int16_t acc_y,
                          const int16_t acc_z,
                          const int16_t gyro_x,
                          const int16_t gyro_y,
                          const int16_t gyro_z,
                          const int16_t mag_x,
                          const int16_t mag_y,
                          const int16_t mag_z)
{
  return aeroc2_msg_imu_raw_pack(msg, sys_id, comp_id,
                                 acc_x, acc_y, acc_z,
                                 gyro_x, gyro_y, gyro_z,
                                 mag_x, mag_y, mag_z);
}


/**
 * Get the acc_x field from the message.
 *
 * @param[in] msg The message.
 * @return The x-axis acceleration in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_acc_x(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload)[0];
  g.c[0] = (msg->payload)[1];
  return (int16_t)g.i;
}


/**
 * Get the acc_y field from the message.
 *
 * @param[in] msg The message.
 * @return The y-axis acceleration in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_acc_y(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + sizeof(int16_t))[0];
  g.c[0] = (msg->payload + sizeof(int16_t))[1];
  return (int16_t)g.i;
}


/**
 * Get the acc_z field from the message.
 *
 * @param[in] msg The message.
 * @return The z-axis acceleration in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_acc_z(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + (2 * sizeof(int16_t)))[0];
  g.c[0] = (msg->payload + (2 * sizeof(int16_t)))[1];
  return (int16_t)g.i;
}


/**
 * Get the gyro_x field from the message.
 *
 * @param[in] msg The message.
 * @return The x-axis angular velocity in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_gyro_x(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + (3 * sizeof(int16_t)))[0];
  g.c[0] = (msg->payload + (3 * sizeof(int16_t)))[1];
  return (int16_t)g.i;
}


/**
 * Get the gyro_y field from the message.
 *
 * @param[in] msg The message.
 * @return The y-axis angular velocity in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_gyro_y(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + (4 * sizeof(int16_t)))[0];
  g.c[0] = (msg->payload + (4 * sizeof(int16_t)))[1];
  return (int16_t)g.i;
}


/**
 * Get the gyro_z field from the message.
 *
 * @param[in] msg The message.
 * @return The z-axis angular velocity in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_gyro_z(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + (5 * sizeof(int16_t)))[0];
  g.c[0] = (msg->payload + (5 * sizeof(int16_t)))[1];
  return (int16_t)g.i;
}


/**
 * Get the mag_x field from the message.
 *
 * @param[in] msg The message.
 * @return The x-axis magnetic flux density in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_mag_x(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + (6 * sizeof(int16_t)))[0];
  g.c[0] = (msg->payload + (6 * sizeof(int16_t)))[1];
  return (int16_t)g.i;
}


/**
 * Get the mag_y field from the message.
 *
 * @param[in] msg The message.
 * @return The y-axis magnetic flux density in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_mag_y(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + (7 * sizeof(int16_t)))[0];
  g.c[0] = (msg->payload + (7 * sizeof(int16_t)))[1];
  return (int16_t)g.i;
}


/**
 * Get the mag_z field from the message.
 *
 * @param[in] msg The message.
 * @return The z-axis magnetic flux density in ADC units.
 */
static inline int16_t
aeroc2_msg_imu_raw_get_mag_z(const aeroc2_message_t* msg)
{
  __generic16_t g;
  g.c[1] = (msg->payload + (8 * sizeof(int16_t)))[0];
  g.c[0] = (msg->payload + (8 * sizeof(int16_t)))[1];
  return (int16_t)g.i;
}


/**
 * Decode the full raw IMU message.
 *
 * @param[out] imu Storage for the decoded message.
 * @param[in]  msg The message to decode.
 */
static inline void
aeroc2_msg_imu_raw_decode(aeroc2_imu_raw_t* imu,
                          const aeroc2_message_t* msg)
{
  imu->acc_x = aeroc2_msg_imu_raw_get_acc_x(msg);
  imu->acc_y = aeroc2_msg_imu_raw_get_acc_y(msg);
  imu->acc_z = aeroc2_msg_imu_raw_get_acc_z(msg);
  imu->gyro_x = aeroc2_msg_imu_raw_get_gyro_x(msg);
  imu->gyro_y = aeroc2_msg_imu_raw_get_gyro_y(msg);
  imu->gyro_z = aeroc2_msg_imu_raw_get_gyro_z(msg);
  imu->mag_x = aeroc2_msg_imu_raw_get_mag_x(msg);
  imu->mag_y = aeroc2_msg_imu_raw_get_mag_y(msg);
  imu->mag_z = aeroc2_msg_imu_raw_get_mag_z(msg);
}


#ifdef __cplusplus
}
#endif

#endif /* AEROC2_MSG_IMU_RAW_H_ */
