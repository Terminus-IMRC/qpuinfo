#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "v3d_rw.h"

#define gt(n, sr) ((((1<<((n)))-1)<<((sr))))
#define en(e, n, sr) ((((p[((e))]&((gt(((n)), ((sr)))))))>>((sr))))
#define rf(offset_in_1_byte, bit_from, bit_to) ((en(((offset_in_1_byte))/(32/8), ((bit_from))-((bit_to))+1, ((bit_to)))))
#define rgt(bit_from, bit_to) ((gt(((bit_from))-((bit_to))+1, ((bit_to)))))
#define renm(fname) (((p[v3d_reg_addr_map[fname].offset]&v3d_reg_addr_map[fname].mask)>>v3d_reg_addr_map[fname].sr))

typedef enum {
	RW_WO = 0x1, RW_RO = 0x2, RW_RW = 0x3
} rwtype_t;

struct {
	int offset;
	uint32_t mask;
	int sr;
	char *name;
	rwtype_t rw;
} v3d_reg_addr_map[V3D_NFNAME];

#define _STR(x) #x
#define STR(x) _STR(x)

#define ram_add_node(fname_suf, offset_in_byte, bit_from, bit_to, bit_rw) \
	v3d_reg_addr_map[V3D_##fname_suf].offset = ((offset_in_byte))/(32/8); \
	v3d_reg_addr_map[V3D_##fname_suf].mask = rgt(((bit_from)), ((bit_to))); \
	v3d_reg_addr_map[V3D_##fname_suf].sr = ((bit_to)); \
	v3d_reg_addr_map[V3D_##fname_suf].name = STR(V3D_##fname_suf); \
	v3d_reg_addr_map[V3D_##fname_suf].rw = bit_rw;

void v3d_init()
{
	ram_add_node(TVER, 0x00000, 31, 24, RW_RO);
	ram_add_node(IDSTR_V, 0x00000, 7, 0, RW_RO);
	ram_add_node(IDSTR_3, 0x00000, 15, 8, RW_RO);
	ram_add_node(IDSTR_D, 0x00000, 23, 16, RW_RO);

	ram_add_node(VPMSZ,0x00004, 31, 28, RW_RO);
	ram_add_node(HDRT, 0x00004, 27, 24, RW_RO);
	ram_add_node(NSEM, 0x00004, 23, 16, RW_RO);
	ram_add_node(TUPS, 0x00004, 15, 12, RW_RO);
	ram_add_node(QUPS, 0x00004, 11, 8, RW_RO);
	ram_add_node(NSLC, 0x00004, 7, 4, RW_RO);
	ram_add_node(REV, 0x00004, 3, 0, RW_RO);
}

uint32_t v3d_read(uint32_t *p, v3d_field_name_t fname)
{
	if ((fname < 0) || (fname >= V3D_NFNAME)) {
		fprintf(stderr, "%s:%d: error: field name out of range: %d\n", __FILE__, __LINE__, fname);
		exit(EXIT_FAILURE);
	} else if (v3d_reg_addr_map[fname].rw == RW_WO) {
		fprintf(stderr, "%s:%d: error: write only register: %d\n", __FILE__, __LINE__, fname);
		exit(EXIT_FAILURE);
	}

	return renm(fname);
}
