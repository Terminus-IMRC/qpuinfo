#include <stdint.h>
#include <stdio.h>
#include "mapmem.h"

#define gt(n, sr) ((((1<<((n)))-1)<<((sr))))
#define en(e, n, sr) ((((p[((e))]&((gt(((n)), ((sr)))))))>>((sr))))

int main()
{
	uint32_t *p;

	p=mapmem();

	printf("p: %p\n", p);
	printf("p[0]: 0x%x\n", p[0]);

	printf("V3D Technology Version: %u\n", en(0, 31-24+1, 24));
	printf("V3D Id String: 0x%x 0x%x 0x%x\n", en(0, 8, 16), en(0, 8, 8), en(0, 8, 0));
	printf("V3D Id String: %c%c%c\n", en(0, 8, 16), en(0, 8, 8), en(0, 8, 0));
	printf("VPM Memory Size: %u\n", en(1, 4, 28));

	return 0;
}
