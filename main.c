#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "mapmem.h"

#define gt(n, sr) ((((1<<((n)))-1)<<((sr))))
#define en(e, n, sr) ((((p[((e))]&((gt(((n)), ((sr)))))))>>((sr))))
#define rf(offset, bit_from, bit_to) ((en(((offset)), ((bit_from))-((bit_to))+1, ((bit_to)))))

int main()
{
	uint32_t *p;

	p=mapmem();

	printf("p: %p\n", p);
	printf("p[0]: 0x%08"PRIx32"\n", p[0]);

	printf("V3D Technology Version: %"PRIu32"\n", rf(0, 31, 24));
	printf("V3D Id String: 0x%02"PRIx8" 0x%02"PRIx8" 0x%02"PRIx8"\n", rf(0, 7, 0), rf(0, 15, 8), rf(0, 23, 16));
	printf("V3D Id String: %c%c%c\n", rf(0, 7, 0), rf(0, 15, 8), rf(0, 23, 16));
	printf("VPM Memory Size: %"PRIu32"\n", rf(1, 31, 28));

	return 0;
}
