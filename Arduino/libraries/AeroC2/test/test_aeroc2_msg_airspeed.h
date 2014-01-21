#ifndef TEST_AEROC2_MSG_AIRSPEED_H_
#define TEST_AEROC2_MSG_AIRSPEED_H_

#include <stdint.h>
#include <CUnit/CUnit.h>
#include "../inc/aeroc2.h"

static inline void
test_aeroc2_msg_airspeed_pack(void)
{
  int len;
  aeroc2_message_t msg;
  
  len = aeroc2_msg_airspeed_pack(&msg, 100, AEROC2_COMP_ID_AIRSPEED, 21.345);
  
  CU_ASSERT_EQUAL(msg.len, 4);
  CU_ASSERT_EQUAL(msg.sys, 100);
  CU_ASSERT_EQUAL(msg.comp, AEROC2_COMP_ID_AIRSPEED);
  CU_ASSERT_EQUAL(msg.type, AEROC2_TYPE_ID_AIRSPEED);
  CU_ASSERT_EQUAL(msg.payload[0], 0x41);
  CU_ASSERT_EQUAL(msg.payload[1], 0xAA);
  CU_ASSERT_EQUAL(msg.payload[2], 0xC2);
  CU_ASSERT_EQUAL(msg.payload[3], 0x8F);
  CU_ASSERT_EQUAL(len, 13);
}


static inline void
test_aeroc2_msg_airspeed_encode(void)
{
  int len;
  aeroc2_message_t msg;
  
  len = aeroc2_msg_airspeed_encode(&msg, 100, AEROC2_COMP_ID_AIRSPEED, 21.345);
  
  CU_ASSERT_EQUAL(msg.len, 4);
  CU_ASSERT_EQUAL(msg.sys, 100);
  CU_ASSERT_EQUAL(msg.comp, AEROC2_COMP_ID_AIRSPEED);
  CU_ASSERT_EQUAL(msg.type, AEROC2_TYPE_ID_AIRSPEED);
  CU_ASSERT_EQUAL(msg.payload[0], 0x41);
  CU_ASSERT_EQUAL(msg.payload[1], 0xAA);
  CU_ASSERT_EQUAL(msg.payload[2], 0xC2);
  CU_ASSERT_EQUAL(msg.payload[3], 0x8F);
  CU_ASSERT_EQUAL(len, 13);
}


static inline void
test_aeroc2_msg_airspeed_get_airspeed(void)
{
  float airspeed;
  aeroc2_message_t msg;
  
  aeroc2_msg_airspeed_encode(&msg, 100, AEROC2_COMP_ID_AIRSPEED, 21.345);
  airspeed = aeroc2_msg_airspeed_get_airspeed(&msg);
  
  CU_ASSERT_DOUBLE_EQUAL(airspeed, 21.345, 1e-5);
}


static inline void
test_aeroc2_msg_airspeed_decode(void)
{
  aeroc2_message_t msg;
  aeroc2_airspeed_t airspeed;
  
  aeroc2_msg_airspeed_encode(&msg, 100, AEROC2_COMP_ID_AIRSPEED, 21.345);
  aeroc2_msg_airspeed_decode(&airspeed, &msg);
  
  CU_ASSERT_DOUBLE_EQUAL(airspeed.airspeed, 21.345, 1e-5);
}


#endif  /* TEST_AEROC2_MSG_AIRSPEED_H_ */
