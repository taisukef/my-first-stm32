static void memclear(uint8* p, int len) {
	for (int i = len; i > 0; i--)
		*p++ = 0;
}
static void memcopy(uint8* dst, const uint8* src, int len) {
	for (int i = len; i > 0; i--)
		*dst++ = *src++;
}

extern unsigned int __data_section_table;

static inline void c_init() {
	// clear RAM
	memclear((uint8*)SRAM_LOC, SRAM_LEN);
	// copy init value to RAM
	uint* ad = &__data_section_table;
	memcopy((uint8*)SRAM_LOC, (uint8*)ad[0], ad[1]);
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
