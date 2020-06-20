/*! 
 * @author: Joel Klimont
 * @filename: types.h
 * @date: 21/06/20
*/

#ifndef TERMINALTRAINS_TYPES_H
#define TERMINALTRAINS_TYPES_H

#include <cstdint>
#include <cstddef>

typedef unsigned int uint;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

static_assert(sizeof(u64) == sizeof(size_t), "size_t is not 64 bit");

#endif //TERMINALTRAINS_TYPES_H
