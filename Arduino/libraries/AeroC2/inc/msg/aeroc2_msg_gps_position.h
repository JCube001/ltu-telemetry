#ifndef AEROC2_MSG_GPS_POSITION_H_
#define AEROC2_MSG_GPS_POSITION_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * GPS position data.
 *
 * @note Degrees are the primary angle unit for GPS.
 *       http://astro.unl.edu/naap/motion1/tc_units.html
 */
typedef struct
{
  float latitude;      /**< X-axis position (deg). */
  float longitude;     /**< Y-axis position (deg). */
  float altitude;      /**< Z-axis position in meters (m). */
} aeroc2_gps_position_t;


/**
 * Pack a GPS position message.
 *
 * @param[out] msg        The message to pack.
 * @param[in]  sys_id     The system ID.
 * @param[in]  comp_id    The component ID.
 * @param[in]  latitude   X-axis position (deg).
 * @param[in]  longitude  Y-axis position (deg).
 * @param[in]  altitude   Z-axis position in meters (m).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_position_pack(aeroc2_message_t* msg,
                             const uint8_t sys_id,
                             const uint8_t comp_id,
                             const float latitude,
                             const float longitude,
                             const float altitude)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_GPS_POSITION;
  
  i += put_float_at_index(msg->payload, latitude, i);
  i += put_float_at_index(msg->payload, longitude, i);
  i += put_float_at_index(msg->payload, altitude, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a GPS position message.
 *
 * @note Identical to aeroc2_msg_gps_position_pack().
 *
 * @param[out] msg        The message to pack.
 * @param[in]  sys_id     The system ID.
 * @param[in]  comp_id    The component ID.
 * @param[in]  latitude   X-axis position (deg).
 * @param[in]  longitude  Y-axis position (deg).
 * @param[in]  altitude   Z-axis position in meters (m).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_position_encode(aeroc2_message_t* msg,
                               const uint8_t sys_id,
                               const uint8_t comp_id,
                               const float latitude,
                               const float longitude,
                               const float altitude)
{
  return aeroc2_msg_gps_position_pack(msg, sys_id, comp_id,
                                      latitude, longitude, altitude);
}


/**
 * Get the latitude field from the message.
 *
 * @param[in] msg The message.
 * @return X-axis position (deg).
 */
static inline float
aeroc2_msg_gps_position_get_latitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (float)g.f;
}


/**
 * Get the longitude field from the message.
 *
 * @param[in] msg The message.
 * @return Y-axis position (deg).
 */
static inline float
aeroc2_msg_gps_position_get_longitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + sizeof(float))[0];
  g.c[2] = (msg->payload + sizeof(float))[1];
  g.c[1] = (msg->payload + sizeof(float))[2];
  g.c[0] = (msg->payload + sizeof(float))[3];
  return (float)g.f;
}


/**
 * Get the altitude field from the message.
 *
 * @param[in] msg The message.
 * @return Z-axis position in meters (m).
 */
static inline float
aeroc2_msg_gps_position_get_altitude(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload + (2 * sizeof(float)))[0];
  g.c[2] = (msg->payload + (2 * sizeof(float)))[1];
  g.c[1] = (msg->payload + (2 * sizeof(float)))[2];
  g.c[0] = (msg->payload + (2 * sizeof(float)))[3];
  return (float)g.f;
}


/**
 * Decode the full GPS position message.
 *
 * @param[out] gps Storage for the decoded message.
 * @param[in]  msg The message to decode.
 */
static inline void
aeroc2_msg_gps_position_decode(aeroc2_gps_position_t* gps,
                               const aeroc2_message_t* msg)
{
  gps->latitude = aeroc2_msg_gps_position_get_latitude(msg);
  gps->longitude = aeroc2_msg_gps_position_get_longitude(msg);
  gps->altitude = aeroc2_msg_gps_position_get_altitude(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_GPS_POSITION_H_ */
