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

	printf("\n");
	printf("[V3D Pipeline Control and Status]\n");
	printf("Binning Mode Out Of Memory: %"PRIu32"\n", v3d_read(p, V3D_BMOOM));
	printf("Rendering Mode Busy: %"PRIu32"\n", v3d_read(p, V3D_RMBUSY));
	printf("Rendering Mode Active: %"PRIu32"\n", v3d_read(p, V3D_RMACTIVE));
	printf("Binning Mode Busy: %"PRIu32"\n", v3d_read(p, V3D_BMBUSY));
	printf("Binning Mode Active: %"PRIu32"\n", v3d_read(p, V3D_BMACTIVE));

	printf("\n");
	printf("[Binning Mode Flush Count]\n");
	printf("Flush Count: %"PRIu32"\n", v3d_read(p, V3D_BMFCT));

	printf("\n");
	printf("[Rendering Mode Frame Count]\n");
	printf("Frame Count: %"PRIu32"\n", v3d_read(p, V3D_RMFCT));

	printf("\n");
	printf("[Current Address of Binning Memory Pool]\n");
	printf("Current Pool Address: 0x%08"PRIx32"\n", v3d_read(p, V3D_BMPCA));

	printf("\n");
	printf("[Remaining Size of Binning Memory Pool]\n");
	printf("Size of Pool Remaining: %"PRIu32"\n", v3d_read(p, V3D_BMPRS));

	printf("\n");
	printf("[Address of Overspill Binning Memory Block]\n");
	printf("Address of Overspill Memory Block for Binning: 0x%08"PRIx32"\n", v3d_read(p, V3D_BMPOA));

	printf("\n");
	printf("[Size of Overspill Binning Memory Block]\n");
	printf("Size of Overspill Memory Block for Binning: %"PRIu32"\n", v3d_read(p, V3D_BMPOS));

	printf("\n");
	printf("[Binner Debug]\n");
	printf("Disable Clipping: %"PRIu32"\n", v3d_read(p, V3D_CLIPDISA));
	printf("Disable Forwarding in State Cache: %"PRIu32"\n", v3d_read(p, V3D_FWDDISA));

	printf("\n");
	printf("[Reserve QPUs 0-7]\n");
	printf("Reservation settings for QPU 7: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV7));
	printf("Reservation settings for QPU 6: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV6));
	printf("Reservation settings for QPU 5: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV5));
	printf("Reservation settings for QPU 4: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV4));
	printf("Reservation settings for QPU 3: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV3));
	printf("Reservation settings for QPU 2: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV2));
	printf("Reservation settings for QPU 1: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV1));
	printf("Reservation settings for QPU 0: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV0));

	printf("\n");
	printf("[Reserve QPUs 8-15]\n");
	printf("Reservation settings for QPU 15: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV15));
	printf("Reservation settings for QPU 14: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV14));
	printf("Reservation settings for QPU 13: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV13));
	printf("Reservation settings for QPU 12: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV12));
	printf("Reservation settings for QPU 11: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV11));
	printf("Reservation settings for QPU 10: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV10));
	printf("Reservation settings for QPU 9: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV9));
	printf("Reservation settings for QPU 8: 0x%1"PRIx32"\n", v3d_read(p, V3D_QPURSV8));

	printf("\n");
	printf("[QPU Scheduler Control]\n");
	printf("Coordinate Shader Scheduling Bypass Limit: %"PRIu32"\n", v3d_read(p, V3D_CSRBL));
	printf("Vertex Shader Scheduling Bypass Limit: %"PRIu32"\n", v3d_read(p, V3D_VSRBL));

	printf("\n");
	printf("[QPU User Program Request Uniforms Address]\n");
	printf("Uniforms Address: 0x%08"PRIx32"\n", v3d_read(p, V3D_QPURQUA));

	printf("\n");
	printf("[QPU User Program Request Uniforms Length]\n");
	printf("Uniforms Length: %"PRId32"\n", v3d_read(p, V3D_QPURQUL));

	printf("\n");
	printf("[QPU User Program Request Control and Status]\n");
	printf("Count of user programs completed: %"PRId32"\n", v3d_read(p, V3D_QPURQCC));
	printf("Count of user programs requests made: %"PRId32"\n", v3d_read(p, V3D_QPURQCM));
	printf("Queue Error: %"PRId32"\n", v3d_read(p, V3D_QPURQERR));
	printf("Queue Length: %"PRId32"\n", v3d_read(p, V3D_QPURQL));

	printf("\n");
	printf("[VPM Allocator Control]\n");
	printf("Enable VPM allocation timeout: %"PRId32"\n", v3d_read(p, V3D_VPATOEN));
	printf("Enable VPM allocation limits: %"PRId32"\n", v3d_read(p, V3D_VPALIMEN));
	printf("Binning VPM allocation timeout %"PRId32"\n", v3d_read(p, V3D_VPABATO));
	printf("Rendering VPM allocation timeout %"PRId32"\n", v3d_read(p, V3D_VPARATO));
	printf("Binning VPM allocation limit %"PRId32"\n", v3d_read(p, V3D_VPABALIM));
	printf("Rendering VPM allocation limit %"PRId32"\n", v3d_read(p, V3D_VPARALIM));

	printf("\n");
	printf("[VPM base (user) memory reservation]\n");
	printf("VPM memory reserved for user programs: %"PRId32"\n", v3d_read(p, V3D_VPMURSV));

	printf("\n");
	printf("[Performance Counters]\n");
	printf("Performance Counter Enable Bits (n=15): %"PRId32"\n", v3d_read(p, V3D_CTEN15));
	printf("Performance Counter Mapping (n=15): %"PRId32"\n", v3d_read(p, V3D_PCTRS15));
	printf("Performance Count (n=15): %"PRId32"\n", v3d_read(p, V3D_PCTR15));
	printf("Performance Counter Enable Bits (n=14): %"PRId32"\n", v3d_read(p, V3D_CTEN14));
	printf("Performance Counter Mapping (n=14): %"PRId32"\n", v3d_read(p, V3D_PCTRS14));
	printf("Performance Count (n=14): %"PRId32"\n", v3d_read(p, V3D_PCTR14));
	printf("Performance Counter Enable Bits (n=13): %"PRId32"\n", v3d_read(p, V3D_CTEN13));
	printf("Performance Counter Mapping (n=13): %"PRId32"\n", v3d_read(p, V3D_PCTRS13));
	printf("Performance Count (n=13): %"PRId32"\n", v3d_read(p, V3D_PCTR13));
	printf("Performance Counter Enable Bits (n=12): %"PRId32"\n", v3d_read(p, V3D_CTEN12));
	printf("Performance Counter Mapping (n=12): %"PRId32"\n", v3d_read(p, V3D_PCTRS12));
	printf("Performance Count (n=12): %"PRId32"\n", v3d_read(p, V3D_PCTR12));
	printf("Performance Counter Enable Bits (n=11): %"PRId32"\n", v3d_read(p, V3D_CTEN11));
	printf("Performance Counter Mapping (n=11): %"PRId32"\n", v3d_read(p, V3D_PCTRS11));
	printf("Performance Count (n=11): %"PRId32"\n", v3d_read(p, V3D_PCTR11));
	printf("Performance Counter Enable Bits (n=10): %"PRId32"\n", v3d_read(p, V3D_CTEN10));
	printf("Performance Counter Mapping (n=10): %"PRId32"\n", v3d_read(p, V3D_PCTRS10));
	printf("Performance Count (n=10): %"PRId32"\n", v3d_read(p, V3D_PCTR10));
	printf("Performance Counter Enable Bits (n=9): %"PRId32"\n", v3d_read(p, V3D_CTEN9));
	printf("Performance Counter Mapping (n=9): %"PRId32"\n", v3d_read(p, V3D_PCTRS9));
	printf("Performance Count (n=9): %"PRId32"\n", v3d_read(p, V3D_PCTR9));
	printf("Performance Counter Enable Bits (n=8): %"PRId32"\n", v3d_read(p, V3D_CTEN8));
	printf("Performance Counter Mapping (n=8): %"PRId32"\n", v3d_read(p, V3D_PCTRS8));
	printf("Performance Count (n=8): %"PRId32"\n", v3d_read(p, V3D_PCTR8));
	printf("Performance Counter Enable Bits (n=7): %"PRId32"\n", v3d_read(p, V3D_CTEN7));
	printf("Performance Counter Mapping (n=7): %"PRId32"\n", v3d_read(p, V3D_PCTRS7));
	printf("Performance Count (n=7): %"PRId32"\n", v3d_read(p, V3D_PCTR7));
	printf("Performance Counter Enable Bits (n=6): %"PRId32"\n", v3d_read(p, V3D_CTEN6));
	printf("Performance Counter Mapping (n=6): %"PRId32"\n", v3d_read(p, V3D_PCTRS6));
	printf("Performance Count (n=6): %"PRId32"\n", v3d_read(p, V3D_PCTR6));
	printf("Performance Counter Enable Bits (n=5): %"PRId32"\n", v3d_read(p, V3D_CTEN5));
	printf("Performance Counter Mapping (n=5): %"PRId32"\n", v3d_read(p, V3D_PCTRS5));
	printf("Performance Count (n=5): %"PRId32"\n", v3d_read(p, V3D_PCTR5));
	printf("Performance Counter Enable Bits (n=4): %"PRId32"\n", v3d_read(p, V3D_CTEN4));
	printf("Performance Counter Mapping (n=4): %"PRId32"\n", v3d_read(p, V3D_PCTRS4));
	printf("Performance Count (n=4): %"PRId32"\n", v3d_read(p, V3D_PCTR4));
	printf("Performance Counter Enable Bits (n=3): %"PRId32"\n", v3d_read(p, V3D_CTEN3));
	printf("Performance Counter Mapping (n=3): %"PRId32"\n", v3d_read(p, V3D_PCTRS3));
	printf("Performance Count (n=3): %"PRId32"\n", v3d_read(p, V3D_PCTR3));
	printf("Performance Counter Enable Bits (n=2): %"PRId32"\n", v3d_read(p, V3D_CTEN2));
	printf("Performance Counter Mapping (n=2): %"PRId32"\n", v3d_read(p, V3D_PCTRS2));
	printf("Performance Count (n=2): %"PRId32"\n", v3d_read(p, V3D_PCTR2));
	printf("Performance Counter Enable Bits (n=1): %"PRId32"\n", v3d_read(p, V3D_CTEN1));
	printf("Performance Counter Mapping (n=1): %"PRId32"\n", v3d_read(p, V3D_PCTRS1));
	printf("Performance Count (n=1): %"PRId32"\n", v3d_read(p, V3D_PCTR1));
	printf("Performance Counter Enable Bits (n=0): %"PRId32"\n", v3d_read(p, V3D_CTEN0));
	printf("Performance Counter Mapping (n=0): %"PRId32"\n", v3d_read(p, V3D_PCTRS0));
	printf("Performance Count (n=0): %"PRId32"\n", v3d_read(p, V3D_PCTR0));

	return 0;
}
