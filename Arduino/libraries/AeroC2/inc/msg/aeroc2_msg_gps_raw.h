#ifndef AEROC2_MSG_GPS_RAW_H_
#define AEROC2_MSG_GPS_RAW_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Raw GPS data.
 *
 * @note Degrees are the primary angle unit for GPS.
 *       http://astro.unl.edu/naap/motion1/tc_units.html
 */
typedef struct
{
  uint8_t satellites;  /**< The number of visible satellites. */
  uint8_t hdop;        /**< Horizontal diminution of precision (discrete). */
  uint32_t date;       /**< The UTC date (DDMMYY). */
  uint32_t time;       /**< The UTC time (HHMMSSCC). */
  int32_t latitude;    /**< X-axis position (deg * 10e6). */
  int32_t longitude;   /**< Y-axis position (deg * 10e6). */
  int32_t altitude;    /**< Z-axis position in centimeters (cm). */
  int32_t speed;       /**< Ground speed (kn * 10e2). */
  int32_t heading;     /**< Ground heading in degrees (deg * 10e2). */
} aeroc2_gps_raw_t;


/**
 * Pack a raw GPS message.
 *
 * @param[out] msg        The message to pack.
 * @param[in]  sys_id     The system ID.
 * @param[in]  comp_id    The component ID.
 * @param[in]  satellites The number of visible satellites.
 * @param[in]  hdop       Horizontal diminution of precision (discrete).
 * @param[in]  date       The UTC date (DDMMYY).
 * @param[in]  time       The UTC time (HHMMSSCC).
 * @param[in]  latitude   X-axis position (deg * 10e6).
 * @param[in]  longitude  Y-axis position (deg * 10e6).
 * @param[in]  altitude   Z-axis position in meters (cm).
 * @param[in]  speed      Ground speed (kn * 10e2).
 * @param[in]  heading    Ground heading in degrees (deg * 10e2).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_raw_pack(aeroc2_message_t* msg,
                        const uint8_t sys_id,
                        const uint8_t comp_id,
                        const uint8_t satellites,
                        const uint8_t hdop,
                        const uint32_t date,
                        const uint32_t time,
                        const int32_t latitude,
                        const int32_t longitude,
                        const int32_t altitude,
                        const int32_t speed,
                        const int32_t heading)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_GPS_RAW;
  
  i += put_uint8_at_index(msg->payload, satellites, i);
  i += put_uint8_at_index(msg->payload, hdop, i);
  i += put_uint32_at_index(msg->payload, date, i);
  i += put_uint32_at_index(msg->payload, time, i);
  i += put_int32_at_index(msg->payload, latitude, i);
  i += put_int32_at_index(msg->payload, longitude, i);
  i += put_int32_at_index(msg->payload, altitude, i);
  i += put_int32_at_index(msg->payload, speed, i);
  i += put_int32_at_index(msg->payload, heading, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a raw GPS message.
 *
 * @note Identical to aeroc2_msg_gps_raw_pack().
 *
 * @param[out] msg        The message to pack.
 * @param[in]  sys_id     The system ID.
 * @param[in]  comp_id    The component ID.
 * @param[in]  satellites The number of visible satellites.
 * @param[in]  hdop       Horizontal diminution of precision (discrete).
 * @param[in]  date       The UTC date (DDMMYY).
 * @param[in]  time       The UTC time (HHMMSSCC).
 * @param[in]  latitude   X-axis position (deg * 10e6).
 * @param[in]  longitude  Y-axis position (deg * 10e6).
 * @param[in]  altitude   Z-axis position in meters (cm).
 * @param[in]  speed      Ground speed (kn * 10e2).
 * @param[in]  heading    Ground heading in degrees (deg * 10e2).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_raw_encode(aeroc2_message_t* msg,
                          const uint8_t sys_id,
                          const uint8_t comp_id,
                          const uint8_t satellites,
                          const uint8_t hdop,
                          const uint32_t date,
                          const uint32_t time,
                          const int32_t latitude,
                          const int32_t longitude,
                          const int32_t altitude,
                          const int32_t speed,
                          const int32_t heading)
{
  return aeroc2_msg_gps_raw_pack(msg, sys_id, comp_id,
                                 satellites, hdop, date, time, latitude,
                                 longitude, altitude, speed, heading);
}


/**
 * Get the satellites field from the message.
 *
 * @param[in] msg The message.
 * @return The number of visible satellites.
 */
static inline uint8_t
aeroc2_msg_gps_raw_get_satellites(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload)[0];
}


/**
 * Get the HDOP field from the message.
 *
 * @param[in] msg The message.
 * @return Horizontal diminution of precision (discrete).
 */
static inline uint8_t
aeroc2_msg_gps_raw_get_hdop(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload + sizeof(uint8_t))[0];
}


/**
 * Get the date field from the message.
 *
 * @param[in] msg The message.
 * @return The UTC date (DDMMYY).
 */
static inline uint32_t
aeroc2_msg_gps_raw_get_date(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)))[3];
  return (uint32_t)g.s;
}


/**
 * Get the time field from the message.
 *
 * @param[in] msg The message.
 * @return The UTC time (HHMMSSCC).
 */
static inline uint32_t
aeroc2_msg_gps_raw_get_time(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + sizeof(uint32_t))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + sizeof(uint32_t))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + sizeof(uint32_t))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + sizeof(uint32_t))[3];
  return (uint32_t)g.s;
}


/**
 * Get the latitude field from the message.
 *
 * @param[in] msg The message.
 * @return X-axis position (deg * 10e6).
 */
static inline int32_t
aeroc2_msg_gps_raw_get_latitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[3];
  return (int32_t)g.i;
}


/**
 * Get the longitude field from the message.
 *
 * @param[in] msg The message.
 * @return Y-axis position (deg * 10e6).
 */
static inline int32_t
aeroc2_msg_gps_raw_get_longitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(int32_t))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(int32_t))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(int32_t))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(int32_t))[3];
  return (int32_t)g.i;
}


/**
 * Get the altitude field from the message.
 *
 * @param[in] msg The message.
 * @return Z-axis position in centimeters (cm).
 */
static inline int32_t
aeroc2_msg_gps_raw_get_altitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(int32_t)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(int32_t)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(int32_t)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(int32_t)))[3];
  return (int32_t)g.i;
}


/**
 * Get the speed field from the message.
 *
 * @param[in] msg The message.
 * @return Ground speed (kn * 10e2).
 */
static inline int32_t
aeroc2_msg_gps_raw_get_speed(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(int32_t)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(int32_t)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(int32_t)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(int32_t)))[3];
  return (int32_t)g.i;
}


/**
 * Get the heading field from the message.
 *
 * @param[in] msg The message.
 * @return Ground heading in degrees (deg * 10e2).
 */
static inline int32_t
aeroc2_msg_gps_raw_get_heading(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(int32_t)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(int32_t)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(int32_t)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(int32_t)))[3];
  return (int32_t)g.i;
}


/**
 * Decode the full raw GPS message.
 *
 * @param[out] gps Storage for the decoded message.
 * @param[in]  msg The message to decode.
 */
static inline void
aeroc2_msg_gps_raw_decode(aeroc2_gps_raw_t* gps,
                          const aeroc2_message_t* msg)
{
  gps->satellites = aeroc2_msg_gps_raw_get_satellites(msg);
  gps->hdop = aeroc2_msg_gps_raw_get_hdop(msg);
  gps->date = aeroc2_msg_gps_raw_get_date(msg);
  gps->time = aeroc2_msg_gps_raw_get_time(msg);
  gps->latitude = aeroc2_msg_gps_raw_get_latitude(msg);
  gps->longitude = aeroc2_msg_gps_raw_get_longitude(msg);
  gps->altitude = aeroc2_msg_gps_raw_get_altitude(msg);
  gps->speed = aeroc2_msg_gps_raw_get_speed(msg);
  gps->heading = aeroc2_msg_gps_raw_get_heading(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_GPS_RAW_H_ */
