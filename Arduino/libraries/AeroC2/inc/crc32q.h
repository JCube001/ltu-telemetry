/**
 * @file crc32q.h
 * Functions and types for CRC checks.
 *
 * Generated on Sun Dec 22 14:33:33 2013,
 * by pycrc v0.8.1, http://www.tty1.net/pycrc/
 * using the configuration:
 *    Width        = 32
 *    Poly         = 0x814141ab
 *    XorIn        = 0x00000000
 *    ReflectIn    = False
 *    XorOut       = 0x00000000
 *    ReflectOut   = False
 *    Algorithm    = table-driven
 *****************************************************************************/
#ifndef __CRC32Q_H__
#define __CRC32Q_H__

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * The definition of the used algorithm.
 *****************************************************************************/
#define CRC_ALGO_TABLE_DRIVEN 1


/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 32 bits.
 *****************************************************************************/
typedef uint32_t crc32q_t;


/**
 * Static table used for the table_driven implementation.
 *****************************************************************************/
static const crc32q_t crc_table[256] = {
    0x00000000, 0x814141ab, 0x83c3c2fd, 0x02828356,
    0x86c6c451, 0x078785fa, 0x050506ac, 0x84444707,
    0x8cccc909, 0x0d8d88a2, 0x0f0f0bf4, 0x8e4e4a5f,
    0x0a0a0d58, 0x8b4b4cf3, 0x89c9cfa5, 0x08888e0e,
    0x98d8d3b9, 0x19999212, 0x1b1b1144, 0x9a5a50ef,
    0x1e1e17e8, 0x9f5f5643, 0x9dddd515, 0x1c9c94be,
    0x14141ab0, 0x95555b1b, 0x97d7d84d, 0x169699e6,
    0x92d2dee1, 0x13939f4a, 0x11111c1c, 0x90505db7,
    0xb0f0e6d9, 0x31b1a772, 0x33332424, 0xb272658f,
    0x36362288, 0xb7776323, 0xb5f5e075, 0x34b4a1de,
    0x3c3c2fd0, 0xbd7d6e7b, 0xbfffed2d, 0x3ebeac86,
    0xbafaeb81, 0x3bbbaa2a, 0x3939297c, 0xb87868d7,
    0x28283560, 0xa96974cb, 0xabebf79d, 0x2aaab636,
    0xaeeef131, 0x2fafb09a, 0x2d2d33cc, 0xac6c7267,
    0xa4e4fc69, 0x25a5bdc2, 0x27273e94, 0xa6667f3f,
    0x22223838, 0xa3637993, 0xa1e1fac5, 0x20a0bb6e,
    0xe0a08c19, 0x61e1cdb2, 0x63634ee4, 0xe2220f4f,
    0x66664848, 0xe72709e3, 0xe5a58ab5, 0x64e4cb1e,
    0x6c6c4510, 0xed2d04bb, 0xefaf87ed, 0x6eeec646,
    0xeaaa8141, 0x6bebc0ea, 0x696943bc, 0xe8280217,
    0x78785fa0, 0xf9391e0b, 0xfbbb9d5d, 0x7afadcf6,
    0xfebe9bf1, 0x7fffda5a, 0x7d7d590c, 0xfc3c18a7,
    0xf4b496a9, 0x75f5d702, 0x77775454, 0xf63615ff,
    0x727252f8, 0xf3331353, 0xf1b19005, 0x70f0d1ae,
    0x50506ac0, 0xd1112b6b, 0xd393a83d, 0x52d2e996,
    0xd696ae91, 0x57d7ef3a, 0x55556c6c, 0xd4142dc7,
    0xdc9ca3c9, 0x5ddde262, 0x5f5f6134, 0xde1e209f,
    0x5a5a6798, 0xdb1b2633, 0xd999a565, 0x58d8e4ce,
    0xc888b979, 0x49c9f8d2, 0x4b4b7b84, 0xca0a3a2f,
    0x4e4e7d28, 0xcf0f3c83, 0xcd8dbfd5, 0x4cccfe7e,
    0x44447070, 0xc50531db, 0xc787b28d, 0x46c6f326,
    0xc282b421, 0x43c3f58a, 0x414176dc, 0xc0003777,
    0x40005999, 0xc1411832, 0xc3c39b64, 0x4282dacf,
    0xc6c69dc8, 0x4787dc63, 0x45055f35, 0xc4441e9e,
    0xcccc9090, 0x4d8dd13b, 0x4f0f526d, 0xce4e13c6,
    0x4a0a54c1, 0xcb4b156a, 0xc9c9963c, 0x4888d797,
    0xd8d88a20, 0x5999cb8b, 0x5b1b48dd, 0xda5a0976,
    0x5e1e4e71, 0xdf5f0fda, 0xdddd8c8c, 0x5c9ccd27,
    0x54144329, 0xd5550282, 0xd7d781d4, 0x5696c07f,
    0xd2d28778, 0x5393c6d3, 0x51114585, 0xd050042e,
    0xf0f0bf40, 0x71b1feeb, 0x73337dbd, 0xf2723c16,
    0x76367b11, 0xf7773aba, 0xf5f5b9ec, 0x74b4f847,
    0x7c3c7649, 0xfd7d37e2, 0xffffb4b4, 0x7ebef51f,
    0xfafab218, 0x7bbbf3b3, 0x793970e5, 0xf878314e,
    0x68286cf9, 0xe9692d52, 0xebebae04, 0x6aaaefaf,
    0xeeeea8a8, 0x6fafe903, 0x6d2d6a55, 0xec6c2bfe,
    0xe4e4a5f0, 0x65a5e45b, 0x6727670d, 0xe66626a6,
    0x622261a1, 0xe363200a, 0xe1e1a35c, 0x60a0e2f7,
    0xa0a0d580, 0x21e1942b, 0x2363177d, 0xa22256d6,
    0x266611d1, 0xa727507a, 0xa5a5d32c, 0x24e49287,
    0x2c6c1c89, 0xad2d5d22, 0xafafde74, 0x2eee9fdf,
    0xaaaad8d8, 0x2beb9973, 0x29691a25, 0xa8285b8e,
    0x38780639, 0xb9394792, 0xbbbbc4c4, 0x3afa856f,
    0xbebec268, 0x3fff83c3, 0x3d7d0095, 0xbc3c413e,
    0xb4b4cf30, 0x35f58e9b, 0x37770dcd, 0xb6364c66,
    0x32720b61, 0xb3334aca, 0xb1b1c99c, 0x30f08837,
    0x10503359, 0x911172f2, 0x9393f1a4, 0x12d2b00f,
    0x9696f708, 0x17d7b6a3, 0x155535f5, 0x9414745e,
    0x9c9cfa50, 0x1dddbbfb, 0x1f5f38ad, 0x9e1e7906,
    0x1a5a3e01, 0x9b1b7faa, 0x9999fcfc, 0x18d8bd57,
    0x8888e0e0, 0x09c9a14b, 0x0b4b221d, 0x8a0a63b6,
    0x0e4e24b1, 0x8f0f651a, 0x8d8de64c, 0x0ccca7e7,
    0x044429e9, 0x85056842, 0x8787eb14, 0x06c6aabf,
    0x8282edb8, 0x03c3ac13, 0x01412f45, 0x80006eee
};


/**
 * Calculate the initial crc value.
 *
 * @return     The initial crc value.
 *****************************************************************************/
static inline crc32q_t
crc32q_init(void)
{
    return 0x00000000;
}


/**
 * Update the crc value with new data.
 *
 * @param crc      The current crc value.
 * @param data     Pointer to a buffer of @a data_len bytes.
 * @param data_len Number of bytes in the @a data buffer.
 * @return         The updated crc value.
 *****************************************************************************/
static inline crc32q_t
crc32q_update(crc32q_t crc,
              const uint8_t* data,
              size_t data_len)
{
    unsigned int tbl_idx;

    while (data_len--) {
        tbl_idx = ((crc >> 24) ^ *data) & 0xff;
        crc = (crc_table[tbl_idx] ^ (crc << 8)) & 0xffffffff;

        data++;
    }
    return crc & 0xffffffff;
}


/**
 * Calculate the final crc value.
 *
 * @param crc  The current crc value.
 * @return     The final crc value.
 *****************************************************************************/
static inline crc32q_t
crc32q_finalize(crc32q_t crc)
{
    return crc ^ 0x00000000;
}


#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* __CRC32Q_H__ */
