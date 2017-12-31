#ifndef STDDEF_H_
#define STDDEF_H_

#define STRING2(s) STRING(s)
#define STRING(str) #str

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned int uint;
//#define INT32_MAX 0x7fff

#define NULL 0

static void memclear(uint8* p, int len) {
	for (int i = len; i > 0; i--)
		*p++ = 0;
}
static void memcopy(uint8* dst, const uint8* src, int len) {
	for (int i = len; i > 0; i--)
		*dst++ = *src++;
}

// for Arm div
__attribute__ ((unused))
static int calcDiv(int n, int m) {
	int flg = 0;
	if (n < 0) {
		n = -n;
		flg++;
	}
	if (m < 0) {
		m = -m;
		flg = !flg;
	}
	n = (uint)n / (uint)m;
	if (flg)
		return -n;
	return n;
}
__attribute__ ((unused))
static int calcMod(int n, int m) {
	int d = calcDiv(n, m);
	return n - d * m;
}

#endif
