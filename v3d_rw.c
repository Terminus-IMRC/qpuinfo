#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "v3d_rw.h"

#define gt(n, sr) ((((1<<((n)))-1)<<((sr))))
#define en(e, n, sr) ((((p[((e))]&((gt(((n)), ((sr)))))))>>((sr))))
#define rf(offset_in_1_byte, bit_from, bit_to) ((en(((offset_in_1_byte))/(32/8), ((bit_from))-((bit_to))+1, ((bit_to)))))

uint32_t v3d_read(uint32_t *p, v3d_field_name_t fname)
{
	switch (fname) {
		case V3D_TVER:
			return rf(0x00000, 31, 24);
		case V3D_IDSTR_V:
			return rf(0x00000, 7, 0);
		case V3D_IDSTR_3:
			return rf(0x00000, 15, 8);
		case V3D_IDSTR_D:
			return rf(0x00000, 23, 16);

		case V3D_VPMSZ:
			return rf(0x00004, 31, 28);
		case V3D_HDRT:
			return rf(0x00004, 27, 24);
		case V3D_NSEM:
			return rf(0x00004, 23, 16);
		case V3D_TUPS:
			return rf(0x00004, 15, 12);
		case V3D_QUPS:
			return rf(0x00004, 11, 8);
		case V3D_NSLC:
			return rf(0x00004, 7, 4);
		case V3D_REV:
			return rf(0x00004, 3, 0);

		default:
			fprintf(stderr, "%s:%d: error: unknown field name: %d\n", __FILE__, __LINE__, fname);
			exit(EXIT_FAILURE);
	}
}
