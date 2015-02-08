#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "mapmem.h"
#include "v3d_rw.h"

int main()
{
	uint32_t *p;

	p=mapmem();

	printf("p: %p\n", p);
	printf("p[0]: 0x%08"PRIx32"\n", p[0]);

	printf("V3D Technology Version: %"PRIu32"\n", v3d_read(p, V3D_TVER));
	printf("V3D Id String: %c%c%c\n", v3d_read(p, V3D_IDSTR_V), v3d_read(p, V3D_IDSTR_3), v3d_read(p, V3D_IDSTR_D));

	printf("VPM Memory Size: %"PRIu32"\n", v3d_read(p, V3D_VPMSZ));
	printf("HDR Support: %"PRIu32"\n", v3d_read(p, V3D_HDRT));
	printf("Number of Semaphores: %"PRIu32"\n", v3d_read(p, V3D_NSEM));
	printf("Number of TMUs per Slice: %"PRIu32"\n", v3d_read(p, V3D_TUPS));
	printf("Number of QPUs per Slice: %"PRIu32"\n", v3d_read(p, V3D_QUPS));
	printf("Number of Slices: %"PRIu32"\n", v3d_read(p, V3D_NSLC));
	printf("V3D Revision: %"PRIu32"\n", v3d_read(p, V3D_REV));

	return 0;
}
