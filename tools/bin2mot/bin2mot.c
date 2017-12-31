#include <stdio.h>

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("bin2mot [bin file]\n");
		return 1;
	}
	FILE* fp = fopen(argv[1], "rb");
	if (!fp) {
		printf("can't open file\n");
		return 1;
	}
	char buf[30];
	int address = 0x08000000;
	for (;;) {
		int n = fread(buf + 5, 1, 16, fp);
		if (n <= 0) {
			break;
		}
		buf[0] = n + 5;
		buf[1] = (char)(address >> 24);
		buf[2] = (char)((address >> 16) & 0xff);
		buf[3] = (char)((address >> 8) & 0xff);
		buf[4] = (char)(address & 0xff);
		int chk = 0;
		for (int i = 0; i < n + 5; i++) {
			chk += buf[i];
		}
		buf[n + 5] = (char)(~chk & 0xff);
		
		printf("S3");
		for (int i = 0; i < n + 5; i++) {
			printf("%02X", buf[i] & 0xff);
		}
		printf("\n");
		address += n;
	}
	printf("S70408000000F3\n"); // 0x08000000 fixed
	fclose(fp);
	return 0;
}
