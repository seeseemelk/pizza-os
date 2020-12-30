#ifndef STDINT_KERNEL_H_
#define STDINT_KERNEL_H_

extern "C" {
#include <stdint.h>
}

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define KB(n) (n * 1024U)
#define MB(n) (KB(n) * 1024U)
#define GB(n) (MB(n) * 1024U)

#endif
