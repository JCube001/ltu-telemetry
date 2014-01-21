#ifndef AEROC2_MSG_GPS_STATUS_H_
#define AEROC2_MSG_GPS_STATUS_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * GPS status data.
 *
 * @note Degrees are the primary angle unit for GPS.
 *       http://astro.unl.edu/naap/motion1/tc_units.html
 */
typedef struct
{
  uint8_t satellites;  /**< The number of visible satellites. */
  uint8_t hdop;        /**< Horizontal diminution of precision (discrete). */
  /* TODO(JCube001): What else should go here? */
} aeroc2_gps_status_t;


/**
 * Pack a GPS status message.
 *
 * @param[out] msg        The message to pack.
 * @param[in]  sys_id     The system ID.
 * @param[in]  comp_id    The component ID.
 * @param[in]  satellites The number of visible satellites.
 * @param[in]  hdop       Horizontal diminution of precision (discrete).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_status_pack(aeroc2_message_t* msg,
                           const uint8_t sys_id,
                           const uint8_t comp_id,
                           const uint8_t satellites,
                           const uint8_t hdop)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_GPS_STATUS;
  
  i += put_uint8_at_index(msg->payload, satellites, i);
  i += put_uint8_at_index(msg->payload, hdop, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a GPS status message.
 *
 * @param[out] msg        The message to pack.
 * @param[in]  sys_id     The system ID.
 * @param[in]  comp_id    The component ID.
 * @param[in]  satellites The number of visible satellites.
 * @param[in]  hdop       Horizontal diminution of precision (discrete).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_gps_status_encode(aeroc2_message_t* msg,
                             const uint8_t sys_id,
                             const uint8_t comp_id,
                             const uint8_t satellites,
                             const uint8_t hdop)
{
  return aeroc2_msg_gps_status_pack(msg, sys_id, comp_id, satellites, hdop);
}


/**
 * Get the satellites field from the message.
 *
 * @param[in] msg The message.
 * @return The number of visible satellites.
 */
static inline uint8_t
aeroc2_msg_gps_status_get_satellites(const aeroc2_message_t* msg)
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
aeroc2_msg_gps_status_get_hdop(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload + sizeof(uint8_t))[0];
}


/**
 * Decode the full GPS status message.
 *
 * @param[out] gps Storage for the decoded message.
 * @param[in]  msg The message to decode.
 */
static inline void
aeroc2_msg_gps_status_decode(aeroc2_gps_status_t* gps,
                             const aeroc2_message_t* msg)
{
  gps->satellites = aeroc2_msg_gps_status_get_satellites(msg);
  gps->hdop = aeroc2_msg_gps_status_get_hdop(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_GPS_STATUS_H_ */
