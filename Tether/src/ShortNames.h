/*
 * ShortNames.h
 *
 *  Created on:	Nov 22, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SHORTNAMES_H_
#define SRC_SHORTNAMES_H_

#include <cstdint>


typedef std::uint64_t u64;
typedef std::int64_t i64;
typedef std::uint32_t u32;
typedef std::int32_t i32;
typedef std::uint16_t u16;
typedef std::int16_t i16;
typedef std::uint8_t u8;
typedef std::int8_t i8;

typedef long double f128;

typedef u32 syncID;

#define u64(x) (u64)(x)
#define i64(x) (i64)(x)
#define u32(x) (u32)(x)
#define i32(x) (i32)(x)
#define u16(x) (u16)(x)
#define i16(x) (i16)(x)
#define u8(x) (u8)(x)
#define i8(x) (i8)(x)

#define f128(x) (f128)(x)


#endif /* SRC_SHORTNAMES_H_ */
