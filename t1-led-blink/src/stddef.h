#ifndef STDDEF_H_
#define STDDEF_H_

#define NULL 0

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;

static void memclear(uint8* p, int len) {
	for (int i = len; i > 0; i--)
		*p++ = 0;
}
static void memcopy(uint8* dst, const uint8* src, int len) {
	for (int i = len; i > 0; i--)
		*dst++ = *src++;
}

#endif
