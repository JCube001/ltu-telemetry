#ifndef TEST_CRC32Q_H_
#define TEST_CRC32Q_H_

#include <stdint.h>
#include <CUnit/CUnit.h>
#include "../inc/crc32q.h"

/**
 * Test that the CRC initial value is what was expected.
 */
static inline void
test_crc32q_init(void)
{
  CU_ASSERT_EQUAL(crc32q_init(), 0);
}


/**
 * Test computing a valid checksum.
 */
static inline void
test_crc32q_update(void)
{
  const uint8_t input = 0x12;
  uint32_t checksum = crc32q_init();
  
  checksum = crc32q_update(checksum, &input, sizeof(input));
  
  CU_ASSERT_EQUAL(checksum, 0x1B1B1144);
}


/**
 * Test computing a valid checksum from a large data source.
 */
static inline void
test_crc32q_update_long(void)
{
  const uint8_t input[] = {
    0xDF, 0xAA, 0x2A, 0xEC, 0xA3, 0x2D, 0xDC, 0xDC,
    0x3D, 0xD4, 0x62, 0x11, 0x2C, 0x1D, 0x43, 0x77,
    0x2B, 0xBA, 0x27, 0x50, 0xC1, 0xD8, 0x93, 0x6E,
    0x17, 0xC6, 0x5C, 0x56, 0x75, 0x35, 0x93, 0xC0
  };
  
  uint32_t checksum = crc32q_init();
  
  checksum = crc32q_update(checksum, input, 32);
  
  CU_ASSERT_EQUAL(checksum, 0x20D4068E);
}


/**
 * Test that the finalize function mutates the checksum as expected.
 */
static inline void
test_crc32q_finalize(void)
{
  const uint8_t input = 0x12;
  uint32_t checksum1, checksum2;

  checksum1 = crc32q_init();
  checksum1 = crc32q_update(checksum1, &input, sizeof(input));
  checksum2 = crc32q_finalize(checksum1);
  
  CU_ASSERT_EQUAL(checksum2, checksum1);
}


#endif  /* TEST_CRC32Q_H_ */
