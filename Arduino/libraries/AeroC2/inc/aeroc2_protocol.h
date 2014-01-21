/*******************************************************************************
AeroC2
Copyright (C) 2014  Jacob McGladdery

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef AEROC2_PROTOCOL_H_
#define AEROC2_PROTOCOL_H_

#include <stdint.h>
#include <string.h>

#include "aeroc2_types.h"
#include "crc32q.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Checksum union.
 */
union __checksum_
{
  uint8_t  c[4];
  uint32_t s;
  crc32q_t crc;
};


/**
 * @addtogroup Finalize
 * @{
 */


/**
 * Finalizes a message.
 *
 * Adds information to the header which can only be known after the payload has
 * been set. The @a sys_id and the @a component_id must be set by the user.
 *
 * @note It is the receiving systems responsibility to check the SEQ number in
 *       order to determine the number of lost packets.
 *
 * @param[out] msg     The message to finalize.
 * @param[in]  sys_id  The system ID.
 * @param[in]  comp_id The component ID.
 * @param[in]  length  The length of the stored payload.
 * @return The total length of the finalized message.
 */
static inline size_t
aeroc2_finalize_message(aeroc2_message_t* msg,
                        const uint8_t sys_id,
                        const uint8_t comp_id,
                        const uint16_t length)
{
  static uint8_t seq = 0;  /* Unique to each message in a sequence */
  size_t i = length + AEROC2_HEADER_LEN;
  
  /* Set the remaining fields of the message */
  msg->len = length;
  msg->sys = sys_id;
  msg->comp = comp_id;
  
  /* Set the current sequence number */
  msg->seq = seq++;
  
  /* Last step, compute the checksum of the message */
  msg->chk = (uint32_t)crc32q_update(0, (uint8_t*)((void*)msg), i);
  i += AEROC2_CHK_LEN;
  
  /* Return the total length of the message */
  return i;
}


/**
 * Copies a message to a byte buffer.
 *
 * @post Sets the value of @a AEROC2_STX as the first element in the buffer.
 * @post Overwrites the original contents of the buffer as needed.
 *
 * @param[out] buffer The byte buffer to copy to.
 * @param[in]  msg    The message to copy from.
 * @return The number of bytes copied to the buffer.
 */
static inline size_t
aeroc2_message_to_buffer(uint8_t* buffer,
                         const aeroc2_message_t* msg)
{
  size_t i = 0;
  uint16_t length = msg->len + AEROC2_HEADER_LEN;
  
  /* Add the message start indicator */
  buffer[i] = AEROC2_STX;
  i += sizeof(buffer[i]);
  
  /* Copy the message bytes into the buffer */
  memcpy(&buffer[i], msg, length);
  i += length;
  
  /* Copy the checksum bytes into the buffer */
  memcpy(&buffer[i], &msg->chk, sizeof(msg->chk));
  i += sizeof(msg->chk);
  
  /* Return the total length of the buffer */
  return i;
}


/**
 * Sets the initial checksum value.
 *
 * @param[out] msg The message to store the checksum in.
 */
static inline void
aeroc2_init_checksum(aeroc2_message_t* msg)
{
  msg->chk = (uint32_t)crc32q_init();
}


/**
 * Updates the checksum value.
 *
 * Accumulates the checksum of one new byte of data; @a c with the checksum
 * already set in @a msg.
 *
 * @param[out] msg The message containing the checksum to be updated.
 * @param[in]  c   The new byte of data to accumulate with the checksum.
 */
static inline void
aeroc2_update_checksum(aeroc2_message_t* msg,
                       uint8_t c)
{
  msg->chk = (uint32_t)crc32q_update(msg->chk, &c, sizeof(c));
}

/** @} */


/**
 * @addtogroup Parsing
 * @{
 */


/**
 * Initializes a parser.
 *
 * Writes zeros to every element of an uninitialized @a aeroc2_status_struct.
 * Switches from the start state to the idle state.
 *
 * @note This function only runs if the status state is out of the range of
 *       valid states.
 * @post Sets the status state to @a AEROC2_PARSE_STATE_IDLE.
 *
 * @param[out] status The status structure to initialize.
 */
static inline void
aeroc2_initialize_parser(aeroc2_status_t* status) 
{
  if ((AEROC2_PARSE_STATE_START >= status->state) ||
      (AEROC2_PARSE_STATE_NUM <= status->state))
  {
    memset(status, 0, sizeof(*status));
    status->state = AEROC2_PARSE_STATE_IDLE;
  }
}


/**
 * Parses one byte through the state machine.
 *
 * Each use of this function advances the finite state machine by one step.
 *
 * @post Changes the state stored in @a status which will affect future runs of
 *       this function with the same @a status object.
 *
 * @param[in]  c      The input for the parser.
 * @param[out] msg    The parsed message.
 * @param[out] status The status of the state machine including metadata.
 * @return A 1 if a complete message was received, otherwise 0.
 */
static inline uint8_t
aeroc2_parse_char(const uint8_t c,
                  aeroc2_message_t* msg,
                  aeroc2_status_t* status)
{
  union __checksum_ chk;
  
  /* Only initializes the status if the current state is out of range */
  aeroc2_initialize_parser(status);
  
  /* Assume no message received before entering the state machine */
  status->msg_received = 0;
  
  switch (status->state)
  {
  /* Check for the start indicator on a packet */
  case AEROC2_PARSE_STATE_START:
  case AEROC2_PARSE_STATE_IDLE:
    if (AEROC2_STX == c)
    {
      aeroc2_init_checksum(msg);
      status->state = AEROC2_PARSE_STATE_GOT_STX;
    }
    break;
  
  /* Got STX */
  case AEROC2_PARSE_STATE_GOT_STX:
    msg->len = c;
    aeroc2_update_checksum(msg, c);
    status->state = AEROC2_PARSE_STATE_GOT_LEN;
    break;
  
  /* Got LEN */
  case AEROC2_PARSE_STATE_GOT_LEN:
    msg->seq = c;
    aeroc2_update_checksum(msg, c);
    status->state = AEROC2_PARSE_STATE_GOT_SEQ;
    break;
  
  /* Got SEQ */
  case AEROC2_PARSE_STATE_GOT_SEQ:
    msg->sys = c;
    aeroc2_update_checksum(msg, c);
    status->state = AEROC2_PARSE_STATE_GOT_SYS;
    break;
  
  /* Got SYS */
  case AEROC2_PARSE_STATE_GOT_SYS:
    msg->comp = c;
    aeroc2_update_checksum(msg, c);
    status->state = AEROC2_PARSE_STATE_GOT_COMP;
    break;
  
  /* Got COMP, check if length equals 0 */
  case AEROC2_PARSE_STATE_GOT_COMP:
    msg->type = c;
    aeroc2_update_checksum(msg, c);
    if (0 == msg->len)
    {
      status->state = AEROC2_PARSE_STATE_GOT_PAYLOAD;
    }
    else
    {
      status->state = AEROC2_PARSE_STATE_GOT_TYPE;
    }
    break;
  
  /* Got TYPE, check for the end of the payload */
  case AEROC2_PARSE_STATE_GOT_TYPE:
    msg->payload[status->payload_index++] = c;
    aeroc2_update_checksum(msg, c);
    if (status->payload_index == msg->len)
    {
      status->payload_index = 0;
      status->state = AEROC2_PARSE_STATE_GOT_PAYLOAD;
    }
    break;
  
  /* Got PAYLOAD, check the checksum */
  case AEROC2_PARSE_STATE_GOT_PAYLOAD:
    chk.s = msg->chk;
    if (c != chk.c[status->chk_index++])
    {
      /* Invalid checksum */
      status->chk = chk.s;
      status->chk_index = 0;
      status->msg_received = 0;
      status->num_parse_error++;
      
      /* There could likely be a new message here */
      if (c == AEROC2_STX)
      {
        aeroc2_init_checksum(msg);
        status->state = AEROC2_PARSE_STATE_GOT_STX;
      }
      else
      {
        status->state = AEROC2_PARSE_STATE_IDLE;
      }
    }
    else if (status->chk_index == AEROC2_CHK_LEN)
    {
      /* Message received */
      status->chk = chk.s;
      status->chk_index = 0;
      status->current_seq++;
      status->msg_received = 1;
      status->num_rx_drop = status->num_parse_error;
      status->num_rx_success++;
      status->state = AEROC2_PARSE_STATE_IDLE;
    }
    break;
  
  /* Default should never be reachable */
  default:
    break;
  }
  
  return status->msg_received;
}

/** @} */


/**
 * @addtogroup Serialization
 * @{
 */


/**
 * Place an unsigned byte into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The byte to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_uint8_at_index(uint8_t* destination,
                   const uint8_t c,
                   const uint8_t index)
{
  destination[index] = c;
  return sizeof(c);
}


/**
 * Place a signed byte into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The byte to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_int8_at_index(uint8_t* destination,
                  const int8_t c,
                  const uint8_t index)
{
  return put_uint8_at_index(destination, (uint8_t)c, index);
}


/**
 * Place 2 unsigned bytes into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The bytes to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_uint16_at_index(uint8_t* destination,
                    const uint16_t c,
                    const uint8_t index)
{
  destination[index]     = (c >> 8) & 0xFF;
  destination[index + 1] = (c & 0xFF);
  return sizeof(c);
}


/**
 * Place 2 signed bytes into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The bytes to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_int16_at_index(uint8_t* destination,
                   const int16_t c,
                   const uint8_t index)
{
  return put_uint16_at_index(destination, (uint16_t)c, index);
}


/**
 * Place 4 unsigned bytes into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The bytes to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_uint32_at_index(uint8_t* destination,
                    const uint32_t c,
                    const uint8_t index)
{
  destination[index]     = (c >> 24) & 0xFF;
  destination[index + 1] = (c >> 16) & 0xFF;
  destination[index + 2] = (c >>  8) & 0xFF;
  destination[index + 3] = (c & 0xFF);
  return sizeof(c);
}


/**
 * Place 4 signed bytes into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The bytes to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_int32_at_index(uint8_t* destination,
                   const int32_t c,
                   const uint8_t index)
{
  return put_uint32_at_index(destination, (uint32_t)c, index);
}


/**
 * Place 8 unsigned bytes into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The bytes to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_uint64_at_index(uint8_t* destination,
                    const uint64_t c,
                    const uint8_t index)
{
  destination[index]     = (c >> 56) & 0xFF;
  destination[index + 1] = (c >> 48) & 0xFF;
  destination[index + 2] = (c >> 40) & 0xFF;
  destination[index + 3] = (c >> 32) & 0xFF;
  destination[index + 4] = (c >> 24) & 0xFF;
  destination[index + 5] = (c >> 16) & 0xFF;
  destination[index + 6] = (c >>  8) & 0xFF;
  destination[index + 7] = (c & 0xFF);
  return sizeof(c);
}


/**
 * Place 8 signed bytes into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The bytes to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_int64_at_index(uint8_t* destination,
                   const int64_t c,
                   const uint8_t index)
{
  return put_uint64_at_index(destination, (uint64_t)c, index);
}


/**
 * Place a float into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  c           The float to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_float_at_index(uint8_t* destination,
                   const float c,
                   const uint8_t index)
{
  __generic32_t g;
  g.f = c;
  return put_int32_at_index(destination, g.i, index);
}


/**
 * Place an array of bytes into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  source      The array to insert.
 * @param[in]  length      The length of the array to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_array_at_index(uint8_t* destination,
                   const uint8_t* source,
                   const uint8_t length,
                   const uint8_t index)
{
  memcpy(destination + index, source, length);
  return length;
}


/**
 * Place a string into the buffer.
 *
 * @warning Does not check for buffer overflow.
 *
 * @param[out] destination The output buffer.
 * @param[in]  source      The string to insert.
 * @param[in]  index       The position to insert at.
 * @return The number of bytes placed into the buffer.
 */
static inline size_t
put_string_at_index(uint8_t* destination,
                    const char* source,
                    const uint8_t index)
{
  strcpy((char*)destination + index, source);
  return strlen(source);
}

/** @} */


#ifdef __cplusplus
}
#endif

#endif  /* AEROC2_PROTOCOL_H_ */
