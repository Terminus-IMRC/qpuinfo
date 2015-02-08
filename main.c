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
	printf("V3D Id String: %c%c%c\n", rf(0, 7, 0), rf(0, 15, 8), rf(0, 23, 16));

	printf("VPM Memory Size: %"PRIu32"\n", rf(1, 31, 28));
	printf("HDR Support: %"PRIu32"\n", rf(1, 27, 24));
	printf("Number of Semaphores: %"PRIu32"\n", rf(1, 23, 16));
	printf("Number of TMUs per Slice: %"PRIu32"\n", rf(1, 15, 12));
	printf("Number of QPUs per Slice: %"PRIu32"\n", rf(1, 11, 8));
	printf("Number of Slices: %"PRIu32"\n", rf(1, 7, 4));
	printf("V3D Revision: %"PRIu32"\n", rf(1, 3, 0));

	return 0;
}
