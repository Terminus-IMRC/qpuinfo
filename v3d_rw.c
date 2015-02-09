#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "v3d_rw.h"

#define gt(n, sr) ((((((n))>=32?0:(1<<((n))))-1)<<((sr))))
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

	ram_add_node(TLBDB, 0x00008, 11, 8, RW_RO);
	ram_add_node(TLBSZ, 0x00008, 7, 4, RW_RO);
	ram_add_node(VRISZ, 0x00008, 3, 0, RW_RO);

	ram_add_node(SCRATCH, 0x00010, 31, 0, RW_RW);

	ram_add_node(L2CCLR, 0x00020, 2, 2, RW_WO);
	ram_add_node(L2CDIS, 0x00020, 1, 1, RW_WO);
	ram_add_node(L2CENA, 0x00020, 0, 0, RW_RW);

	ram_add_node(T1CCS0, 0x00024, 27, 27, RW_RO);
	ram_add_node(T1CCS1, 0x00024, 26, 26, RW_RO);
	ram_add_node(T1CCS2, 0x00024, 25, 25, RW_RO);
	ram_add_node(T1CCS3, 0x00024, 24, 24, RW_RO);
	ram_add_node(T0CCS0, 0x00024, 19, 19, RW_RO);
	ram_add_node(T0CCS1, 0x00024, 18, 18, RW_RO);
	ram_add_node(T0CCS2, 0x00024, 17, 17, RW_RO);
	ram_add_node(T0CCS3, 0x00024, 16, 16, RW_RO);
	ram_add_node(UCCS0, 0x00024, 11, 11, RW_RO);
	ram_add_node(UCCS1, 0x00024, 10, 10, RW_RO);
	ram_add_node(UCCS2, 0x00024, 9, 9, RW_RO);
	ram_add_node(UCCS3, 0x00024, 8, 8, RW_RO);
	ram_add_node(ICCS0, 0x00024, 3, 3, RW_RO);
	ram_add_node(ICCS1, 0x00024, 2, 2, RW_RO);
	ram_add_node(ICCS2, 0x00024, 1, 1, RW_RO);
	ram_add_node(ICCS3, 0x00024, 0, 0, RW_RO);

	ram_add_node(INT_SPILLUSE, 0x00030, 3, 3, RW_RW);
	ram_add_node(INT_OUTTOMEM, 0x00030, 2, 2, RW_RW);
	ram_add_node(INT_FLDONE, 0x00030, 1, 1, RW_RW);
	ram_add_node(INT_FRDONE, 0x00030, 0, 0, RW_RW);

	ram_add_node(EI_SPILLUSE, 0x00034, 3, 3, RW_RW);
	ram_add_node(EI_OUTTOMEM, 0x00034, 2, 2, RW_RW);
	ram_add_node(EI_FLDONE, 0x00034, 1, 1, RW_RW);
	ram_add_node(EI_FRDONE, 0x00034, 0, 0, RW_RW);

	ram_add_node(DI_SPILLUSE, 0x00038, 3, 3, RW_RW);
	ram_add_node(DI_OUTTOMEM, 0x00038, 2, 2, RW_RW);
	ram_add_node(DI_FLDONE, 0x00038, 1, 1, RW_RW);
	ram_add_node(DI_FRDONE, 0x00038, 0, 0, RW_RW);

	ram_add_node(CT0CS_CTRSTA, 0x00100, 15, 15, RW_WO);
	ram_add_node(CT0CS_CTSEMA, 0x00100, 14, 12, RW_RO);
	ram_add_node(CT0CS_CTRTSD, 0x00100, 9, 8, RW_RO);
	ram_add_node(CT0CS_CTRUN, 0x00100, 5, 5, RW_RW);
	ram_add_node(CT0CS_CTSUBS, 0x00100, 4, 4, RW_RW);
	ram_add_node(CT0CS_CTERR, 0x00100, 3, 3, RW_RO);
	ram_add_node(CT0CS_CTMODE, 0x00100, 0, 0, RW_RO);

	ram_add_node(CT1CS_CTRSTA, 0x00104, 15, 15, RW_WO);
	ram_add_node(CT1CS_CTSEMA, 0x00104, 14, 12, RW_RO);
	ram_add_node(CT1CS_CTRTSD, 0x00104, 9, 8, RW_RO);
	ram_add_node(CT1CS_CTRUN, 0x00104, 5, 5, RW_RW);
	ram_add_node(CT1CS_CTSUBS, 0x00104, 4, 4, RW_RW);
	ram_add_node(CT1CS_CTERR, 0x00104, 3, 3, RW_RO);
	ram_add_node(CT1CS_CTMODE, 0x00104, 0, 0, RW_RO);

	ram_add_node(CT0EA_CTLEA, 0x00108, 31, 0, RW_RW);

	ram_add_node(CT1EA_CTLEA, 0x0010c, 31, 0, RW_RW);

	ram_add_node(CT0CA_CTLCA, 0x00110, 31, 0, RW_RW);

	ram_add_node(CT1CA_CTLCA, 0x00114, 31, 0, RW_RW);

	ram_add_node(CT00RA0_CTLRA, 0x00118, 31, 0, RW_RO);

	ram_add_node(CT01RA0_CTLRA, 0x0011c, 31, 0, RW_RO);

	ram_add_node(CT0LC_CTLLCM, 0x00120, 31, 16, RW_RW);
	ram_add_node(CT0LC_CTLSLCS, 0x00120, 15, 0, RW_RW);

	ram_add_node(CT1LC_CTLLCM, 0x00124, 31, 16, RW_RW);
	ram_add_node(CT1LC_CTLSLCS, 0x00124, 15, 0, RW_RW);

	ram_add_node(CT0PC_CTLPC, 0x00128, 31, 0, RW_RO);

	ram_add_node(CT1PC_CTLPC, 0x0012c, 31, 0, RW_RO);

	ram_add_node(BMOOM, 0x00130, 8, 8, RW_RO);
	ram_add_node(RMBUSY, 0x00130, 3, 3, RW_RO);
	ram_add_node(RMACTIVE, 0x00130, 2, 2, RW_RO);
	ram_add_node(BMBUSY, 0x00130, 1, 1, RW_RO);
	ram_add_node(BMACTIVE, 0x00130, 0, 0, RW_RO);

	ram_add_node(BMFCT, 0x00134, 7, 0, RW_RW);

	ram_add_node(RMFCT, 0x00138, 7, 0, RW_RW);

	ram_add_node(BMPCA, 0x00300, 31, 0, RW_RO);

	ram_add_node(BMPRS, 0x00304, 31, 0, RW_RO);

	ram_add_node(BMPOA, 0x00308, 31, 0, RW_RW);

	ram_add_node(BMPOS, 0x0030c, 31, 0, RW_RW);

	ram_add_node(CLIPDISA, 0x00310, 1, 1, RW_RW);
	ram_add_node(FWDDISA, 0x00310, 0, 0, RW_RW);

	ram_add_node(QPURSV7, 0x00410, 31, 28, RW_RW);
	ram_add_node(QPURSV6, 0x00410, 27, 24, RW_RW);
	ram_add_node(QPURSV5, 0x00410, 23, 20, RW_RW);
	ram_add_node(QPURSV4, 0x00410, 19, 16, RW_RW);
	ram_add_node(QPURSV3, 0x00410, 15, 12, RW_RW);
	ram_add_node(QPURSV2, 0x00410, 11, 8, RW_RW);
	ram_add_node(QPURSV1, 0x00410, 7, 4, RW_RW);
	ram_add_node(QPURSV0, 0x00410, 3, 0, RW_RW);

	ram_add_node(QPURSV15, 0x00414, 31, 28, RW_RW);
	ram_add_node(QPURSV14, 0x00414, 27, 24, RW_RW);
	ram_add_node(QPURSV13, 0x00414, 23, 20, RW_RW);
	ram_add_node(QPURSV12, 0x00414, 19, 16, RW_RW);
	ram_add_node(QPURSV11, 0x00414, 15, 12, RW_RW);
	ram_add_node(QPURSV10, 0x00414, 11, 8, RW_RW);
	ram_add_node(QPURSV9, 0x00414, 7, 4, RW_RW);
	ram_add_node(QPURSV8, 0x00414, 3, 0, RW_RW);

	ram_add_node(CSRBL, 0x00418, 3, 2, RW_RW);
	ram_add_node(VSRBL, 0x00418, 1, 0, RW_RW);

	ram_add_node(QPURQPC, 0x00430, 31, 0, RW_WO);

	ram_add_node(QPURQUA, 0x00434, 31, 0, RW_RW);

	ram_add_node(QPURQUL, 0x00438, 11, 0, RW_RW);

	ram_add_node(QPURQCC, 0x0043c, 23, 16, RW_RW);
	ram_add_node(QPURQCM, 0x0043c, 15, 8, RW_RW);
	ram_add_node(QPURQERR, 0x0043c, 7, 7, RW_RW);
	ram_add_node(QPURQL, 0x0043c, 5, 0, RW_RW);

	ram_add_node(VPATOEN, 0x00500, 13, 13, RW_RW);
	ram_add_node(VPALIMEN, 0x00500, 12, 12, RW_RW);
	ram_add_node(VPABATO, 0x00500, 11, 9, RW_RW);
	ram_add_node(VPARATO, 0x00500, 8, 6, RW_RW);
	ram_add_node(VPABALIM, 0x00500, 5, 3, RW_RW);
	ram_add_node(VPARALIM, 0x00500, 2, 0, RW_RW);

	ram_add_node(VPMURSV, 0x00504, 4, 0, RW_RW);
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
