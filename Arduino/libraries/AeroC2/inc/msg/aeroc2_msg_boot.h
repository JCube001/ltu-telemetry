#ifndef AEROC2_MSG_BOOT_H_
#define AEROC2_MSG_BOOT_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Boot data.
 */
typedef struct
{
  uint32_t version;  /**< The aircraft software version. */
} aeroc2_boot_t;


/**
 * Pack a boot message.
 *
 * @param[out] msg     The message to pack.
 * @param[in]  sys_id  The system ID.
 * @param[in]  comp_id The component ID.
 * @param[in]  version The aircraft software version.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_boot_pack(aeroc2_message_t* msg,
                     const uint8_t sys_id,
                     const uint8_t comp_id,
                     const uint32_t version)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_BOOT;
  
  i += put_uint32_at_index(msg->payload, version, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a boot message.
 *
 * @note Identical to aeroc2_msg_boot_pack().
 *
 * @param[out] msg     The message to pack.
 * @param[in]  sys_id  The system ID.
 * @param[in]  comp_id The component ID.
 * @param[in]  version The aircraft software version.
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_boot_encode(aeroc2_message_t* msg,
                       const uint8_t sys_id,
                       const uint8_t comp_id,
                       const uint32_t version)
{
  return aeroc2_msg_boot_pack(msg, sys_id, comp_id, version);
}


/**
 * Get the version field from the message.
 *
 * @param[in] msg The message.
 * @return The version number.
 */
static inline uint32_t
aeroc2_msg_boot_get_version(const aeroc2_message_t* msg)
{
  __generic32_t g;
  g.c[3] = (msg->payload)[0];
  g.c[2] = (msg->payload)[1];
  g.c[1] = (msg->payload)[2];
  g.c[0] = (msg->payload)[3];
  return (uint32_t)g.s;
}


/**
 * Decode the full boot message.
 *
 * @param[out] boot Storage for the decoded message.
 * @param[in]  msg  The message to decode.
 */
static inline void
aeroc2_msg_boot_decode(aeroc2_boot_t* boot,
                       const aeroc2_message_t* msg)
{
  boot->version = aeroc2_msg_boot_get_version(msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_BOOT_H_ */
