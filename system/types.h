#ifndef __TYPES_H__
#define	__TYPES_H__

#include<sys/types.h>

typedef	u_int8_t	u8;
typedef	u_int16_t	u16;
typedef	u_int32_t	u32;
typedef	u_int64_t	u64;

typedef	u_int8_t	uint8_t;
typedef	u_int16_t	uint16_t;
typedef	u_int32_t	uint32_t;
typedef	u_int64_t	uint64_t;

typedef	unsigned long		ULONG;

#define 	BYTE		u8
#define 	WORD		u16
#define 	DWORD		u32
#define 	QWORD		u64

inline int Min(int a, int b){return (a<b)?a:b;};

#endif
