#include <CUnit/CUCurses.h>
#include "test_aeroc2_core.h"
#include "test_aeroc2_msg_airspeed.h"
#include "test_aeroc2_msg_boot.h"
#include "test_aeroc2_msg_keep_alive.h"
#include "test_crc32q.h"


int main(void) {
  CU_TestInfo tests_aeroc2_core[] = {
    {"Finalize message", test_aeroc2_finalize_message},
    {"Message to buffer", test_aeroc2_message_to_buffer},
    {"Initialize checksum", test_aeroc2_init_checksum},
    {"Update checksum", test_aeroc2_update_checksum},
    {"Initialize parser", test_aeroc2_initialize_parser},
    {"Initialize existing parser", test_aeroc2_initialize_existing_parser},
    {"Parse one byte at a time", test_aeroc2_parse_char},
    {"Parse a full message", test_aeroc2_parse_full_message},
    {"Put 1 unsigned byte in a buffer", test_put_uint8_at_index},
    {"Put 1 signed byte in a buffer", test_put_int8_at_index},
    {"Put 2 unsigned bytes in a buffer", test_put_uint16_at_index},
    {"Put 2 signed bytes in a buffer", test_put_int16_at_index},
    {"Put 4 unsigned bytes in a buffer", test_put_uint32_at_index},
    {"Put 4 signed bytes in a buffer", test_put_int32_at_index},
    {"Put 8 unsigned bytes in a buffer", test_put_uint64_at_index},
    {"Put 8 signed bytes in a buffer", test_put_int64_at_index},
    {"Put a float in a buffer", test_put_float_at_index},
    {"Put an array of bytes in a buffer", test_put_array_at_index},
    {"Put a string in a buffer", test_put_string_at_index},
    CU_TEST_INFO_NULL
  };
  
  CU_TestInfo tests_aeroc2_msg_airspeed[] = {
    {"Pack message", test_aeroc2_msg_airspeed_pack},
    {"Encode message", test_aeroc2_msg_airspeed_encode},
    {"Get airspeed", test_aeroc2_msg_airspeed_get_airspeed},
    {"Decode message", test_aeroc2_msg_airspeed_decode}
  };
  
  /*
  CU_TestInfo tests_aeroc2_msg_boot[] = {
  };
  
  CU_TestInfo tests_aeroc2_msg_keep_alive[] = {
  };
  */
  
  CU_TestInfo tests_crc32q[] = {
    {"Initialize", test_crc32q_init},
    {"Update", test_crc32q_update},
    {"Update long", test_crc32q_update_long},
    {"Finalize", test_crc32q_finalize},
    CU_TEST_INFO_NULL
  };
  
  CU_SuiteInfo suites[] = {
    {"AeroC2 Core", NULL, NULL, tests_aeroc2_core},
    {"AeroC2 Message Airspeed", NULL, NULL, tests_aeroc2_msg_airspeed},
    //{"AeroC2 Message Boot", NULL, NULL, tests_aeroc2_msg_boot},
    //{"AeroC2 Message Keep Alive", NULL, NULL, tests_aeroc2_msg_keep_alive},
    {"CRC-32Q", NULL, NULL, tests_crc32q},
    CU_SUITE_INFO_NULL
  };
  
  /* Initialize the test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
  {
    return CU_get_error();
  }
  
  /* Add the suites to the registry */
  if (CUE_SUCCESS != CU_register_suites(suites))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  /* Run all tests using the curses interface */
  CU_curses_run_tests();
  
  /* Done testing, clean up registry and return */
  CU_cleanup_registry();
  return CU_get_error();
}
