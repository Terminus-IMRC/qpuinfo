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

	v3d_init();

	printf("[V3D Identification 0 (V3D block identity)]\n");
	printf("V3D Technology Version: %"PRIu32"\n", v3d_read(p, V3D_TVER));
	printf("V3D Id String: %c%c%c\n", v3d_read(p, V3D_IDSTR_V), v3d_read(p, V3D_IDSTR_3), v3d_read(p, V3D_IDSTR_D));

	printf("\n");
	printf("[V3D Identification 1 (V3D Configuration A)]\n");
	printf("VPM Memory Size: %"PRIu32"\n", v3d_read(p, V3D_VPMSZ));
	printf("HDR Support: %"PRIu32"\n", v3d_read(p, V3D_HDRT));
	printf("Number of Semaphores: %"PRIu32"\n", v3d_read(p, V3D_NSEM));
	printf("Number of TMUs per Slice: %"PRIu32"\n", v3d_read(p, V3D_TUPS));
	printf("Number of QPUs per Slice: %"PRIu32"\n", v3d_read(p, V3D_QUPS));
	printf("Number of Slices: %"PRIu32"\n", v3d_read(p, V3D_NSLC));
	printf("V3D Revision: %"PRIu32"\n", v3d_read(p, V3D_REV));

	printf("\n");
	printf("[V3D Identification 1 (V3D Configuration B)]\n");
	printf("Tile Buffer Double-buffer Mode Support: %"PRIu32"\n", v3d_read(p, V3D_TLBDB));
	printf("Tile Buffer Size: %"PRIu32"\n", v3d_read(p, V3D_TLBSZ));
	printf("VRI Memory Size: %"PRIu32"\n", v3d_read(p, V3D_VRISZ));

	printf("\n");
	printf("[Scratch Register]\n");
	printf("Scratch Register: 0x%08"PRIx32"\n", v3d_read(p, V3D_SCRATCH));

	printf("\n");
	printf("[L2 Cache Control]\n");
	printf("L2 Cache Enable: %"PRIu32"\n", v3d_read(p, V3D_L2CENA));

	printf("\n");
	printf("[Interrupt Control]\n");
	printf("Binner Used Overspill Memory intterrupt status: %"PRIu32"\n", v3d_read(p, V3D_INT_SPILLUSE));
	printf("Binner Out of Memory intterrupt status: %"PRIu32"\n", v3d_read(p, V3D_INT_OUTTOMEM));
	printf("Binning Mode Flush Done intterrupt status: %"PRIu32"\n", v3d_read(p, V3D_INT_FLDONE));
	printf("Render Mode Frame Done interrupt status: %"PRIu32"\n", v3d_read(p, V3D_INT_FRDONE));

	printf("\n");
	printf("[Interrupt Enables]\n");
	printf("Binner Used Overspill Memory intterrupt enable: %"PRIu32"\n", v3d_read(p, V3D_EI_SPILLUSE));
	printf("Binner Out of Memory intterrupt enable: %"PRIu32"\n", v3d_read(p, V3D_EI_OUTTOMEM));
	printf("Binning Mode Flush Done intterrupt enable: %"PRIu32"\n", v3d_read(p, V3D_EI_FLDONE));
	printf("Render Mode Frame Done interrupt enable: %"PRIu32"\n", v3d_read(p, V3D_EI_FRDONE));

	printf("\n");
	printf("[Interrupt Disables]\n");
	printf("Binner Used Overspill Memory intterrupt disable: %"PRIu32"\n", v3d_read(p, V3D_DI_SPILLUSE));
	printf("Binner Out of Memory intterrupt disable: %"PRIu32"\n", v3d_read(p, V3D_DI_OUTTOMEM));
	printf("Binning Mode Flush Done intterrupt disable: %"PRIu32"\n", v3d_read(p, V3D_DI_FLDONE));
	printf("Render Mode Frame Done interrupt disable: %"PRIu32"\n", v3d_read(p, V3D_DI_FRDONE));

	printf("\n");
	printf("[Control List Executor Thread 0 Control and Status]\n");
	printf("Counting Semaphore: %"PRIu32"\n", v3d_read(p, V3D_CT0CS_CTSEMA));
	printf("Return Stack Depth: %"PRIu32"\n", v3d_read(p, V3D_CT0CS_CTRTSD));
	printf("Control Thread Run: %"PRIu32"\n", v3d_read(p, V3D_CT0CS_CTRUN));
	printf("Control Thread Sub-mode: %"PRIu32"\n", v3d_read(p, V3D_CT0CS_CTSUBS));
	printf("Control Thread Error: %"PRIu32"\n", v3d_read(p, V3D_CT0CS_CTERR));
	printf("Control Thread Mode (binning mode thread only): %"PRIu32"\n", v3d_read(p, V3D_CT0CS_CTMODE));

	printf("\n");
	printf("[Control List Executor Thread 1 Control and Status]\n");
	printf("Counting Semaphore: %"PRIu32"\n", v3d_read(p, V3D_CT1CS_CTSEMA));
	printf("Return Stack Depth: %"PRIu32"\n", v3d_read(p, V3D_CT1CS_CTRTSD));
	printf("Control Thread Run: %"PRIu32"\n", v3d_read(p, V3D_CT1CS_CTRUN));
	printf("Control Thread Sub-mode: %"PRIu32"\n", v3d_read(p, V3D_CT1CS_CTSUBS));
	printf("Control Thread Error: %"PRIu32"\n", v3d_read(p, V3D_CT1CS_CTERR));
	printf("Control Thread Mode (binning mode thread only): %"PRIu32"\n", v3d_read(p, V3D_CT1CS_CTMODE));

	printf("\n");
	printf("[Control List Executor Thread 0 End Address]\n");
	printf("Control List End Address: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT0EA_CTLEA));

	printf("\n");
	printf("[Control List Executor Thread 1 End Address]\n");
	printf("Control List End Address: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT1EA_CTLEA));

	printf("\n");
	printf("[Control List Executor Thread 0 Current Address]\n");
	printf("Control List Current Address: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT0CA_CTLCA));

	printf("\n");
	printf("[Control List Executor Thread 1 Current Address]\n");
	printf("Control List Current Address: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT1CA_CTLCA));

	printf("\n");
	printf("[Control List Executor Thread 0 Return Address 0]\n");
	printf("Control List Return Address 0: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT00RA0_CTLRA));

	printf("\n");
	printf("[Control List Executor Thread 1 Return Address 0]\n");
	printf("Control List Return Address 0: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT01RA0_CTLRA));

	printf("\n");
	printf("[Control List Executor Thread 0 List Counter]\n");
	printf("Major List Counter: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT0LC_CTLLCM));
	printf("Sub-list Counter: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT0LC_CTLSLCS));

	printf("\n");
	printf("[Control List Executor Thread 1 List Counter]\n");
	printf("Major List Counter: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT1LC_CTLLCM));
	printf("Sub-list Counter: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT1LC_CTLSLCS));

	printf("\n");
	printf("[Control List Executor Thread 0 Primitive List Counter]\n");
	printf("Primitive List Counter: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT0PC_CTLPC));

	printf("\n");
	printf("[Control List Executor Thread 1 Primitive List Counter]\n");
	printf("Primitive List Counter: 0x%08"PRIx32"\n", v3d_read(p, V3D_CT1PC_CTLPC));

	return 0;
}
