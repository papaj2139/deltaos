#ifndef LIB_CRC32_H
#define LIB_CRC32_H

#include <arch/types.h>

//ISO 3309 / ITU-T V.42 CRC-32 (the standard one used by GPT)
//polynomial: 0x04C11DB7 (reflected: 0xEDB88320)

//compute CRC32 over a buffer, updating a running value
//for a fresh computation, pass CRC32_INIT as prev, then XOR result with CRC32_FINAL
uint32 crc32_update(uint32 prev, const void *data, size len);

//convenience wrapper: compute CRC32 of a buffer in one call
uint32 crc32(const void *data, size len);

#define CRC32_INIT  0xFFFFFFFFu
#define CRC32_FINAL 0xFFFFFFFFu

#endif
