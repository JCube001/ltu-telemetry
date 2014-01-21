#ifndef AEROC2_MSG_STATUS_TEXT_H_
#define AEROC2_MSG_STATUS_TEXT_H_

#include <stdint.h>
#include "../aeroc2_protocol.h"
#include "aeroc2_type_id.h"

#define STATUS_MAX_LEN  (50)

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Status data.
 */
typedef struct
{
  uint8_t severity;  /**< The level of severity; 0: Info, 255: Critical. */
  char text[STATUS_MAX_LEN];  /**< The status message string (ASCII). */
} aeroc2_status_text_t;


/**
 * Pack a status message.
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  severity The level of severity; 0: Info, 255: Critical.
 * @param[in]  text     The status message string (ASCII).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_status_pack(aeroc2_message_t* msg,
                       const uint8_t sys_id,
                       const uint8_t comp_id,
                       const uint8_t severity,
                       const char* text)
{
  size_t i = 0;
  
  msg->type = AEROC2_TYPE_ID_STATUS_TEXT;
  
  i += put_uint8_at_index(msg->payload, severity, i);
  i += put_string_at_index(msg->payload, text, i);
  
  return aeroc2_finalize_message(msg, sys_id, comp_id, i);
}


/**
 * Encode a status message.
 *
 * @note Identical to aeroc2_msg_status_pack().
 *
 * @param[out] msg      The message to pack.
 * @param[in]  sys_id   The system ID.
 * @param[in]  comp_id  The component ID.
 * @param[in]  severity The level of severity; 0: Info, 255: Critical.
 * @param[in]  text     The status message string (ASCII).
 * @return The length of the message in bytes excluding the STX marker.
 */
static inline size_t
aeroc2_msg_status_encode(aeroc2_message_t* msg,
                         const uint8_t sys_id,
                         const uint8_t comp_id,
                         const uint8_t severity,
                         const char* text)
{
  return aeroc2_msg_status_pack(msg, sys_id, comp_id, severity, text);
}


/**
 * Get the severity field from the message.
 *
 * @param[in] msg The message.
 * @return The level of severity; 0: Info, 255: Critical.
 */
static inline uint8_t
aeroc2_msg_status_get_severity(const aeroc2_message_t* msg)
{
  return (uint8_t)(msg->payload)[0];
}


/**
 * Get the text field from the message.
 *
 * @param[out] str Storage for the status message string (ASCII).
 * @param[in]  msg The message.
 * @return The maximum size of the status message string.
 */
static inline size_t
aeroc2_msg_status_get_text(char* str,
                           const aeroc2_message_t* msg)
{
  memcpy(str, msg->payload + sizeof(uint8_t), STATUS_MAX_LEN);
  return STATUS_MAX_LEN;
}


/**
 * Decode the full status message.
 *
 * @param[out] status Storage for the decoded message.
 * @param[in]  msg    The message to decode.
 */
static inline void
aeroc2_msg_status_decode(aeroc2_status_text_t* status_text,
                         const aeroc2_message_t* msg)
{
  status_text->severity = aeroc2_msg_status_get_severity(msg);
  aeroc2_msg_status_get_text(status_text->text, msg);
}


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_MSG_STATUS_TEXT_H_ */
