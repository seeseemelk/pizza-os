/*
 * cdefs.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef CDEFS_H_
#define CDEFS_H_
#include <stdint.h>

//#define KB(x) ((size_t) (x) << 10)
//#define MB(x) ((size_t) (x) << 20)
#define KB(x) ((size_t) x * 1024)
#define MB(x) (KB(x) * 1024)
#define GB(x) (MB(x) * 1024)

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define UNUSED(x) (void)(x)


#endif /* CDEFS_H_ */
