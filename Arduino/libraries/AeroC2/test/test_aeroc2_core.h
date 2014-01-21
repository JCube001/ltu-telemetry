#ifndef TEST_AEROC2_CORE_H_
#define TEST_AEROC2_CORE_H_

#include <stdint.h>
#include <CUnit/CUnit.h>
#include "../inc/aeroc2.h"


/**
 * Test finalizing a message.
 */
static inline void
test_aeroc2_finalize_message(void)
{
  int ret;
  aeroc2_message_t msg;
  
  msg.payload[0] = 0x1;
  
  ret = aeroc2_finalize_message(&msg, 0, 0, 1);
  
  CU_ASSERT_EQUAL(msg.len, 1);
  /* The sequence number is static */
  CU_ASSERT_EQUAL(msg.sys, 0);
  CU_ASSERT_EQUAL(msg.comp, 0);
  CU_ASSERT_EQUAL(msg.payload[0], 1);
  CU_ASSERT_NOT_EQUAL((msg.chk >> 24) & 0xFF, 0);
  CU_ASSERT_NOT_EQUAL((msg.chk >> 16) & 0xFF, 0);
  CU_ASSERT_NOT_EQUAL((msg.chk >>  8) & 0xFF, 0);
  CU_ASSERT_NOT_EQUAL((msg.chk & 0xFF), 0);
  CU_ASSERT_EQUAL(ret, 9 + 1);
}


/**
 * Test copying a message object to a byte buffer.
 */
static inline void
test_aeroc2_message_to_buffer(void)
{
  int ret;
  uint8_t buf[512];
  aeroc2_message_t msg;
  
  msg.payload[0] = 0x1;
  msg.payload[1] = 0x2;
  msg.payload[2] = 0x3;
  
  aeroc2_finalize_message(&msg, 0, 0, 3);
  ret = aeroc2_message_to_buffer(buf, &msg);
  
  CU_ASSERT_EQUAL(buf[0], AEROC2_STX);
  CU_ASSERT_EQUAL(buf[1], 3);
  /* Index 2: The sequence number is static */
  CU_ASSERT_EQUAL(buf[3], 0);
  CU_ASSERT_EQUAL(buf[4], 0);
  CU_ASSERT_EQUAL(buf[5], 0);
  CU_ASSERT_EQUAL(buf[6], 1);
  CU_ASSERT_EQUAL(buf[7], 2);
  CU_ASSERT_EQUAL(buf[8], 3);
  CU_ASSERT_NOT_EQUAL(buf[9], 0);
  CU_ASSERT_NOT_EQUAL(buf[10], 0);
  CU_ASSERT_NOT_EQUAL(buf[11], 0);
  CU_ASSERT_NOT_EQUAL(buf[12], 0);
  CU_ASSERT_EQUAL(ret, 10 + 3);
}


/**
 * Test the initial value of the checksum.
 */
static inline void
test_aeroc2_init_checksum(void)
{
  aeroc2_message_t msg;
  
  aeroc2_init_checksum(&msg);
  
  CU_ASSERT_EQUAL(msg.chk, 0);
}


/**
 * Test accumulating an update on a checksum.
 */
static inline void
test_aeroc2_update_checksum(void)
{
  aeroc2_message_t msg;
  
  aeroc2_init_checksum(&msg);
  aeroc2_update_checksum(&msg, 1);
  
  CU_ASSERT_EQUAL(msg.chk, 0x814141AB);
  
  /* Check that the checksum accumulates the same number applied again */
  aeroc2_update_checksum(&msg, 1);
  
  CU_ASSERT_EQUAL(msg.chk, 0x0141F299);
}


/**
 * Test initializing the parser status object.
 */
static inline void
test_aeroc2_initialize_parser(void)
{
  aeroc2_status_t s;
  
  aeroc2_initialize_parser(&s);
  
  CU_ASSERT_EQUAL(s.chk, 0);
  CU_ASSERT_EQUAL(s.chk_index, 0);
  CU_ASSERT_EQUAL(s.current_seq, 0);
  CU_ASSERT_EQUAL(s.msg_received, 0);
  CU_ASSERT_EQUAL(s.num_parse_error, 0);
  CU_ASSERT_EQUAL(s.num_rx_drop, 0);
  CU_ASSERT_EQUAL(s.num_rx_success, 0);
  CU_ASSERT_EQUAL(s.payload_index, 0);
  CU_ASSERT_EQUAL(s.state, AEROC2_PARSE_STATE_IDLE);
}


/**
 * Test initializing an already initialized status object.
 */
static inline void
test_aeroc2_initialize_existing_parser(void)
{
  aeroc2_status_t s;
  
  /* Run on new status */
  aeroc2_initialize_parser(&s);
  
  /* Mutate the status */
  s.chk = 0xFF;
  s.chk_index = 0xFF;
  s.current_seq = 0xFF;
  s.msg_received = 0xFF;
  s.num_parse_error = 0xFF;
  s.num_rx_drop = 0xFF;
  s.num_rx_success = 0xFF;
  s.payload_index = 0xFF;
  
  /* Run on existing status, no change should occur */
  aeroc2_initialize_parser(&s);
  
  CU_ASSERT_EQUAL(s.chk, 0xFF);
  CU_ASSERT_EQUAL(s.chk_index, 0xFF);
  CU_ASSERT_EQUAL(s.current_seq, 0xFF);
  CU_ASSERT_EQUAL(s.msg_received, 0xFF);
  CU_ASSERT_EQUAL(s.num_parse_error, 0xFF);
  CU_ASSERT_EQUAL(s.num_rx_drop, 0xFF);
  CU_ASSERT_EQUAL(s.num_rx_success, 0xFF);
  CU_ASSERT_EQUAL(s.payload_index, 0xFF);
  CU_ASSERT_EQUAL(s.state, AEROC2_PARSE_STATE_IDLE);
}


/**
 * Test parsing and check the status one byte at a time.
 */
static inline void
test_aeroc2_parse_char(void)
{
  aeroc2_message_t msg;
  aeroc2_status_t status;
  
  /* Compute the CRC ahead of time */
  union crc_u {
    uint8_t  c[4];
    uint32_t s;
  };
  
  union crc_u crc;
  
  uint8_t buf[] = {1, 0, 0, 0, 0, 0};
  crc.s = crc32q_update(0, buf, 6);
  
  /* Check that no state transition occured since STX has not been seen yet */
  aeroc2_parse_char(0, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_IDLE);
  
  /* Check the transition was made to "Got STX" */
  aeroc2_parse_char(AEROC2_STX, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_STX);
  
  /* Check the transition was made to "Got LEN" */
  aeroc2_parse_char(1, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_LEN);
  
  /* Check the transition was made to "Got SEQ" */
  aeroc2_parse_char(0, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_SEQ);
  
  /* Check the transition was made to "Got SYS" */
  aeroc2_parse_char(0, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_SYS);
  
  /* Check the transition was made to "Got COMP" */
  aeroc2_parse_char(0, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_COMP);
  
  /* Check the transition was made to "Got TYPE" */
  aeroc2_parse_char(0, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_TYPE);
  
  /* Check the transition was made to "Got PAYLOAD" */
  aeroc2_parse_char(0, &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_PAYLOAD);
  
  /* Parse the checksum, we know this a priori in this test */
  aeroc2_parse_char(crc.c[0], &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_PAYLOAD);
  
  aeroc2_parse_char(crc.c[1], &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_PAYLOAD);
  
  aeroc2_parse_char(crc.c[2], &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_GOT_PAYLOAD);
  
  aeroc2_parse_char(crc.c[3], &msg, &status);
  CU_ASSERT_EQUAL(status.state, AEROC2_PARSE_STATE_IDLE);
  
  /* Check that a valid message was received */
  CU_ASSERT_TRUE(status.msg_received);
  CU_ASSERT_EQUAL(status.current_seq, 1);
  CU_ASSERT_EQUAL(status.num_rx_drop, 0);
  CU_ASSERT_EQUAL(status.num_rx_success, 1);
}


/**
 * Test parsing an entire message all at once.
 */
static inline void
test_aeroc2_parse_full_message(void)
{
  int i, ret;
  aeroc2_message_t msg;
  aeroc2_status_t status;
  
  /* Compute the CRC ahead of time */
  union crc_u {
    uint8_t  c[4];
    uint32_t s;
  };
  
  union crc_u crc;
  
  uint8_t data[] = {
    0x0A, 0x01, 0x02, 0x03, 0x04,  /* Header */
    0x01, 0x02, 0x03, 0x04, 0x05,  /* Payload 1-5 */
    0x06, 0x07, 0x08, 0x09, 0x0A   /* Payload 6-10 */
  };
  crc.s = crc32q_update(0, data, 15);
  
  /* A simple valid test message with a 10 byte payload */
  uint8_t buf[] = {
    0x55, 0x0A, 0x01, 0x02, 0x03, 0x04,     /* STX + header */
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,     /* Payload 1-6 */
    0x07, 0x08, 0x09, 0x0A,                 /* Payload 7-10 */
    crc.c[0], crc.c[1], crc.c[2], crc.c[3]  /* CRC */
  };
  
  /* Loop through the entire buffer */
  for (i = 0; i < 20; i++)
  {
    ret = aeroc2_parse_char(buf[i], &msg, &status);
  }
  
  CU_ASSERT_TRUE(ret);
  CU_ASSERT_EQUAL(msg.len, data[0]);
  CU_ASSERT_EQUAL(msg.seq, data[1]);
  CU_ASSERT_EQUAL(msg.sys, data[2]);
  CU_ASSERT_EQUAL(msg.comp, data[3]);
  CU_ASSERT_EQUAL(msg.type, data[4]);
  CU_ASSERT_EQUAL(msg.payload[0], data[5]);
  CU_ASSERT_EQUAL(msg.payload[1], data[6]);
  CU_ASSERT_EQUAL(msg.payload[2], data[7]);
  CU_ASSERT_EQUAL(msg.payload[3], data[8]);
  CU_ASSERT_EQUAL(msg.payload[4], data[9]);
  CU_ASSERT_EQUAL(msg.payload[5], data[10]);
  CU_ASSERT_EQUAL(msg.payload[6], data[11]);
  CU_ASSERT_EQUAL(msg.payload[7], data[12]);
  CU_ASSERT_EQUAL(msg.payload[8], data[13]);
  CU_ASSERT_EQUAL(msg.payload[9], data[14]);
  CU_ASSERT_EQUAL(msg.chk, crc.s);
}


/**
 * Test that the buffer gets new data and that the number of bytes consumed by
 * the new data is returned.
 */
static inline void
test_put_uint8_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_uint8_at_index(buf, 0xFF, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(ret, sizeof(uint8_t));
}


/**
 * Test that the buffer gets new data with a signed 8-bit int.
 */
static inline void
test_put_int8_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_int8_at_index(buf, 0xFF, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(ret, sizeof(int8_t));
}


/**
 * Test that the buffer gets new data with an unsigned 16-bit int.
 */
static inline void test_put_uint16_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_uint16_at_index(buf, 0xFFEE, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(buf[i + 1], 0xEE);
  CU_ASSERT_EQUAL(ret, sizeof(uint16_t));
}


/**
 * Test that the buffer gets new data with a signed 16-bit int.
 */
void test_put_int16_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_int16_at_index(buf, 0xFFEE, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(buf[i + 1], 0xEE);
  CU_ASSERT_EQUAL(ret, sizeof(int16_t));
}


/**
 * Test that the buffer gets new data with an unsigned 32-bit int.
 */
static inline void
test_put_uint32_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_uint32_at_index(buf, 0xFFEEDDCC, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(buf[i + 1], 0xEE);
  CU_ASSERT_EQUAL(buf[i + 2], 0xDD);
  CU_ASSERT_EQUAL(buf[i + 3], 0xCC);
  CU_ASSERT_EQUAL(ret, sizeof(uint32_t));
}


/**
 * Test that the buffer gets new data with a signed 32-bit int.
 */
static inline void
test_put_int32_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_int32_at_index(buf, 0xFFEEDDCC, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(buf[i + 1], 0xEE);
  CU_ASSERT_EQUAL(buf[i + 2], 0xDD);
  CU_ASSERT_EQUAL(buf[i + 3], 0xCC);
  CU_ASSERT_EQUAL(ret, sizeof(int32_t));
}


/**
 * Test that the buffer gets new data with an unsigned 64-bit int.
 */
static inline void
test_put_uint64_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_uint64_at_index(buf, 0xFFEEDDCCBBAA9988, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(buf[i + 1], 0xEE);
  CU_ASSERT_EQUAL(buf[i + 2], 0xDD);
  CU_ASSERT_EQUAL(buf[i + 3], 0xCC);
  CU_ASSERT_EQUAL(buf[i + 4], 0xBB);
  CU_ASSERT_EQUAL(buf[i + 5], 0xAA);
  CU_ASSERT_EQUAL(buf[i + 6], 0x99);
  CU_ASSERT_EQUAL(buf[i + 7], 0x88);
  CU_ASSERT_EQUAL(ret, sizeof(uint64_t));
}


/**
 * Test that the buffer gets new data with a signed 64-bit int.
 */
static inline void
test_put_int64_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_int64_at_index(buf, 0xFFEEDDCCBBAA9988, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(buf[i + 1], 0xEE);
  CU_ASSERT_EQUAL(buf[i + 2], 0xDD);
  CU_ASSERT_EQUAL(buf[i + 3], 0xCC);
  CU_ASSERT_EQUAL(buf[i + 4], 0xBB);
  CU_ASSERT_EQUAL(buf[i + 5], 0xAA);
  CU_ASSERT_EQUAL(buf[i + 6], 0x99);
  CU_ASSERT_EQUAL(buf[i + 7], 0x88);
  CU_ASSERT_EQUAL(ret, sizeof(int64_t));
}


/**
 * Test that the buffer gets new data with a float.
 */
static inline void
test_put_float_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  
  ret = put_float_at_index(buf, 187.327861, i);
  
  /* The expected value was computed externally */
  CU_ASSERT_EQUAL(buf[i], 0x43);
  CU_ASSERT_EQUAL(buf[i + 1], 0x3B);
  CU_ASSERT_EQUAL(buf[i + 2], 0x53);
  CU_ASSERT_EQUAL(buf[i + 3], 0xEF);
  CU_ASSERT_EQUAL(ret, sizeof(float));
}


/**
 * Test that the buffer gets new data from an array.
 */
static inline void
test_put_array_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  uint8_t arr[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB};
  
  ret = put_array_at_index(buf, arr, 5, i);
  
  CU_ASSERT_EQUAL(buf[i], 0xFF);
  CU_ASSERT_EQUAL(buf[i + 1], 0xEE);
  CU_ASSERT_EQUAL(buf[i + 2], 0xDD);
  CU_ASSERT_EQUAL(buf[i + 3], 0xCC);
  CU_ASSERT_EQUAL(buf[i + 4], 0xBB);
  CU_ASSERT_EQUAL(ret, 5);
}


/**
 * Test that the buffer get new data from a string.
 */
static inline void
test_put_string_at_index(void)
{
  int i = 10;
  int ret;
  uint8_t buf[AEROC2_MAX_PAYLOAD_LEN];
  char str[] = "Hello, World!";
  
  ret = put_string_at_index(buf, str, i);
  
  CU_ASSERT_STRING_EQUAL((char*)buf + i, str);
  CU_ASSERT_EQUAL(ret, strlen(str));
}


#endif  /* TEST_AEROC2_CORE_H_ */
