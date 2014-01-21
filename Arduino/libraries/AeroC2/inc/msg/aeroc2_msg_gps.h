#ifndef AEROC2_MSG_GPS_H_
#define AEROC2_MSG_GPS_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * GPS data.
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
  float latitude;      /**< X-axis position (deg). */
  float longitude;     /**< Y-axis position (deg). */
  float altitude;      /**< Z-axis position in meters (m). */
  float speed;         /**< Ground speed (m/s). */
  float heading;       /**< Ground heading in degrees (deg). */
} aeroc2_gps_t;


/**
 * Pack a GPS message.
 *
 * @param[out] msg        The message to pack.
 * @param[in]  sys_id     The system ID.
 * @param[in]  comp_id    The component ID.
 * @param[in]  satellites The number of visible satellites.
 * @param[in]  hdop       Horizontal diminution of precision (discrete).
 * @param[in]  date       The UTC date (DDMMYY).
 * @param[in]  time       The UTC time (HHMMSSCC).
 * @param[in]  latitude   X-axis position (deg).
 * @param[in]  longitude  Y-axis position (deg).
 * @param[in]  altitude   Z-axis position in meters (m).
 * @param[in]  speed      Ground speed (m/s).
 * @param[in]  heading    Ground heading in degrees (deg).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_pack(aeroc2_message_t* msg,
                    const uint8_t sys_id,
                    const uint8_t comp_id,
                    const uint8_t satellites,
                    const uint8_t hdop,
                    const uint32_t date,
                    const uint32_t time,
                    const float latitude,
                    const float longitude,
                    const float altitude,
                    const float speed,
                    const float heading)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_GPS;
  
  i += put_uint8_at_index(msg->payload, satellites, i);
  i += put_uint8_at_index(msg->payload, hdop, i);
  i += put_uint32_at_index(msg->payload, date, i);
  i += put_uint32_at_index(msg->payload, time, i);
  i += put_float_at_index(msg->payload, latitude, i);
  i += put_float_at_index(msg->payload, longitude, i);
  i += put_float_at_index(msg->payload, altitude, i);
  i += put_float_at_index(msg->payload, speed, i);
  i += put_float_at_index(msg->payload, heading, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a GPS message.
 *
 * @note Identical to aeroc2_msg_gps_pack().
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  satellites The number of visible satellites.
 * @param[in]  hdop       Horizontal diminution of precision (discrete).
 * @param[in]  date       The UTC date (DDMMYY).
 * @param[in]  time       The UTC time (HHMMSSCC).
 * @param[in]  latitude   X-axis position (deg).
 * @param[in]  longitude  Y-axis position (deg).
 * @param[in]  altitude   Z-axis position in meters (m).
 * @param[in]  speed      Ground speed (m/s).
 * @param[in]  heading    Ground heading in degrees (deg).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_encode(aeroc2_message_t* msg,
                      const uint8_t sys_id,
                      const uint8_t comp_id,
                      const uint8_t satellites,
                      const uint8_t hdop,
                      const uint32_t date,
                      const uint32_t time,
                      const float latitude,
                      const float longitude,
                      const float altitude,
                      const float speed,
                      const float heading)
{
  return aeroc2_msg_gps_pack(msg, sys_id, comp_id,
                             satellites, hdop, date, time, latitude, longitude,
                             altitude, speed, heading);
}


/**
 * Get the satellites field from the message.
 *
 * @param[in] msg The message.
 * @return The number of visible satellites.
 */
static inline uint8_t
aeroc2_msg_gps_get_satellites(const aeroc2_message_t* msg)
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
aeroc2_msg_gps_get_hdop(const aeroc2_message_t* msg)
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
aeroc2_msg_gps_get_date(const aeroc2_message_t* msg)
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
aeroc2_msg_gps_get_time(const aeroc2_message_t* msg)
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
 * @return X-axis position (deg).
 */
static inline float
aeroc2_msg_gps_get_latitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)))[3];
  return (float)g.f;
}


/**
 * Get the longitude field from the message.
 *
 * @param[in] msg The message.
 * @return Y-axis position (deg).
 */
static inline float
aeroc2_msg_gps_get_longitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(float))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(float))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(float))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            sizeof(float))[3];
  return (float)g.f;
}


/**
 * Get the altitude field from the message.
 *
 * @param[in] msg The message.
 * @return Z-axis position in meters (m).
 */
static inline float
aeroc2_msg_gps_get_altitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (2 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the speed field from the message.
 *
 * @param[in] msg The message.
 * @return Ground speed (m/s).
 */
static inline float
aeroc2_msg_gps_get_speed(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (3 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Get the heading field from the message.
 *
 * @param[in] msg The message.
 * @return Ground heading in degrees (deg).
 */
static inline float
aeroc2_msg_gps_get_heading(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(uint8_t)) + (2 * sizeof(uint32_t)) +
            (4 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Decode the full GPS message.
 *
 * @param[out] gps Storage for the decoded message.
 * @param[in]  msg The message to decode.
 */
static inline void
aeroc2_msg_gps_decode(aeroc2_gps_t* gps,
                      const aeroc2_message_t* msg)
{
  gps->satellites = aeroc2_msg_gps_get_satellites(msg);
  gps->hdop = aeroc2_msg_gps_get_hdop(msg);
  gps->date = aeroc2_msg_gps_get_date(msg);
  gps->time = aeroc2_msg_gps_get_time(msg);
  gps->latitude = aeroc2_msg_gps_get_latitude(msg);
  gps->longitude = aeroc2_msg_gps_get_longitude(msg);
  gps->altitude = aeroc2_msg_gps_get_altitude(msg);
  gps->speed = aeroc2_msg_gps_get_speed(msg);
  gps->heading = aeroc2_msg_gps_get_heading(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_GPS_H_ */
