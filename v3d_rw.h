#ifndef _V3D_RW_H_INCLUDED_
#define _V3D_RW_H_INCLUDED_

#include <stdint.h>

	typedef enum {
		/* 0x00000 */ V3D_TVER, V3D_IDSTR_V, V3D_IDSTR_3, V3D_IDSTR_D,
		/* 0x00004 */ V3D_VPMSZ, V3D_HDRT, V3D_NSEM, V3D_TUPS, V3D_QUPS, V3D_NSLC, V3D_REV,
	} v3d_field_name_t;

	uint32_t v3d_read(uint32_t *p, v3d_field_name_t fname);

#endif /* _V3D_RW_H_INCLUDED_ */
