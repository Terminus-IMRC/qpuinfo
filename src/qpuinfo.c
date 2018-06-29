#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <mailbox.h>
#include <vc4regmap.h>

static void usage(const char * const progname)
{
    fprintf(stderr,
            "Usage: %s [-e] [-d] [-w] [-u]\n"
            "-e to enable QPU at first\n"
            "-d to disable at last\n"
            "-w to print write-only registers\n"
            "-u to print registers which names are not known\n",
            progname);
}

int main(int argc, char *argv[])
{
    int i;
    int fd = -1;
    int opt;
    _Bool flag_enable_qpu = 0, flag_disable_qpu = 0;
    _Bool print_writeonly = 0, print_unknown = 0;
    volatile uint32_t *peri;
    uint32_t v;
    int err;

    while ((opt = getopt(argc, argv, "edwu")) != -1){
        switch (opt) {
            case 'e':
                flag_enable_qpu = !0;
                break;
            case 'd':
                flag_disable_qpu = !0;
                break;
            case 'w':
                print_writeonly = !0;
                break;
            case 'u':
                print_unknown = !0;
                break;
            default:
                fprintf(stderr, "error: unknown option: %c\n", opt);
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (optind < argc) {
        fprintf(stderr, "error: Extra %s specified\n",
                argc - optind > 1 ? "options" : "option");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (flag_enable_qpu || flag_disable_qpu) {
        fd = mailbox_open();
        if (fd == -1) {
            fprintf(stderr, "error: mailbox_open\n");
            exit(EXIT_FAILURE);
        }
    }
    if (flag_enable_qpu) {
        err = mailbox_qpu_enable(fd, 1);
        if (err) {
            fprintf(stderr, "error: mailbox_qpu_enable\n");
            (void) mailbox_close(fd);
            exit(EXIT_FAILURE);
        }
    }

    peri = vc4regmap_init();
    if (peri == NULL) {
        fprintf(stderr, "error: vc4regmap_init\n");
        if (flag_enable_qpu || flag_disable_qpu)
            (void) mailbox_close(fd);
        exit(EXIT_FAILURE);
    }

    printf("QPU is %s\n", is_qpu_enabled(peri) ? "enabled" : "disabled");
    printf("\n");

#define B(msb, lsb) ((uint32_t) ((v >> (lsb)) & ((((uint64_t) 1) << ((msb) - (lsb) + 1)) - 1)))

    /* Ident, not Idein. */

    v = peri[V3D_IDENT0];
    printf("[V3D Identification 0 (V3D block identity)]: 0x%08"PRIx32"\n", v);
    printf("V3D Technology Version: %"PRIu32"\n", B(31, 24));
    printf("V3D Id String: %c%c%c\n", B(7,0), B(15,8), B(23,16));
    printf("\n");

    v = peri[V3D_IDENT1];
    printf("[V3D Identification 1 (V3D Configuration A)]: 0x%08"PRIx32"\n", v);
    printf("VPM Memory Size: %"PRIu32"\n", B(31,28));
    printf("HDR Support: %"PRIu32"\n", B(27,24));
    printf("Number of Semaphores: %"PRIu32"\n", B(23,16));
    printf("Number of TMUs per Slice: %"PRIu32"\n", B(15,12));
    printf("Number of QPUs per Slice: %"PRIu32"\n", B(11,8));
    printf("Number of Slices: %"PRIu32"\n", B(7,4));
    printf("V3D Revision: %"PRIu32"\n", B(3,0));
    printf("\n");

    v = peri[V3D_IDENT2];
    /* This should be 2? */
    printf("[V3D Identification 1 (V3D Configuration B)]: 0x%08"PRIx32"\n", v);
    printf("Tile Buffer Double-buffer Mode Support: %"PRIu32"\n", B(11,8));
    printf("Tile Buffer Size: %"PRIu32"\n", B(7,4));
    printf("VRI Memory Size: %"PRIu32"\n", B(3,0));
    printf("\n");

    if (print_unknown) {
        v = peri[V3D_IDENT3];
        printf("[IDENT3]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    v = peri[V3D_SCRATCH];
    printf("[Scratch Register]: 0x%08"PRIx32"\n", v);
    printf("Scratch Register: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_L2CACTL];
    printf("[L2 Cache Control]: 0x%08"PRIx32"\n", v);
    if (print_writeonly)
        printf("L2 Cache Clear: %"PRIu32"\n", B(2,2));
    if (print_writeonly)
        printf("L2 Cache Disable: %"PRIu32"\n", B(1,1));
    printf("L2 Cache Enable: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_SLCACTL];
    printf("[Slices Cache Control]: 0x%08"PRIx32"\n", v);
    if (print_writeonly)
        printf("TMU1 Cache Clear Bits: 0x%"PRIx32"\n", B(27,24));
    if (print_writeonly)
        printf("TMU0 Cache Clear Bits: 0x%"PRIx32"\n", B(19,16));
    if (print_writeonly)
        printf("Uniforms Cache Clear Bits: 0x%"PRIx32"\n", B(11,8));
    if (print_writeonly)
        printf("Instruction Cache Clear Bits: 0x%"PRIx32"\n", B(3,0));
    printf("\n");

    v = peri[V3D_INTCTL];
    printf("[Interrupt Control]: 0x%08"PRIx32"\n", v);
    printf("Binner Used Overspill Memory intterrupt status: %"PRIu32"\n", B(3,3));
    printf("Binner Out of Memory intterrupt status: %"PRIu32"\n", B(2,2));
    printf("Binning Mode Flush Done intterrupt status: %"PRIu32"\n", B(1,1));
    printf("Render Mode Frame Done interrupt status: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_INTENA];
    printf("[Interrupt Enables]: 0x%08"PRIx32"\n", v);
    printf("Binner Used Overspill Memory intterrupt enable: %"PRIu32"\n", B(3,3));
    printf("Binner Out of Memory intterrupt enable: %"PRIu32"\n", B(2,2));
    printf("Binning Mode Flush Done intterrupt enable: %"PRIu32"\n", B(1,1));
    printf("Render Mode Frame Done interrupt enable: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_INTDIS];
    printf("[Interrupt Disables]: 0x%08"PRIx32"\n", v);
    printf("Binner Used Overspill Memory intterrupt disable: %"PRIu32"\n", B(3,3));
    printf("Binner Out of Memory intterrupt disable: %"PRIu32"\n", B(2,2));
    printf("Binning Mode Flush Done intterrupt disable: %"PRIu32"\n", B(1,1));
    printf("Render Mode Frame Done interrupt disable: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_CT0CS];
    printf("[Control List Executor Thread 0 Control and Status]: 0x%08"PRIx32"\n", v);
    if (print_writeonly)
        printf("Reset bit: %"PRIu32"\n", B(15,15));
    printf("Counting Semaphore: %"PRIu32"\n", B(14,12));
    printf("Return Stack Depth: %"PRIu32"\n", B(9,8));
    printf("Control Thread Run: %"PRIu32"\n", B(5,5));
    printf("Control Thread Sub-mode: %"PRIu32"\n", B(4,4));
    printf("Control Thread Error: %"PRIu32"\n", B(3,3));
    printf("Control Thread Mode (binning mode thread only): %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_CT1CS];
    printf("[Control List Executor Thread 1 Control and Status]: 0x%08"PRIx32"\n", v);
    if (print_writeonly)
        printf("Reset bit: %"PRIu32"\n", B(15,15));
    printf("Counting Semaphore: %"PRIu32"\n", B(14,12));
    printf("Return Stack Depth: %"PRIu32"\n", B(9,8));
    printf("Control Thread Run: %"PRIu32"\n", B(5,5));
    printf("Control Thread Sub-mode: %"PRIu32"\n", B(4,4));
    printf("Control Thread Error: %"PRIu32"\n", B(3,3));
    printf("Control Thread Mode (binning mode thread only): %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_CT0EA];
    printf("[Control List Executor Thread 0 End Address]: 0x%08"PRIx32"\n", v);
    printf("Control List End Address: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_CT1EA];
    printf("[Control List Executor Thread 1 End Address]: 0x%08"PRIx32"\n", v);
    printf("Control List End Address: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_CT0CA];
    printf("[Control List Executor Thread 0 Current Address]: 0x%08"PRIx32"\n", v);
    printf("Control List Current Address: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_CT1CA];
    printf("[Control List Executor Thread 1 Current Address]: 0x%08"PRIx32"\n", v);
    printf("Control List Current Address: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_CT00RA0];
    printf("[Control List Executor Thread 0 Return Address 0]: 0x%08"PRIx32"\n", v);
    printf("Control List Return Address 0: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_CT01RA0];
    printf("[Control List Executor Thread 1 Return Address 0]: 0x%08"PRIx32"\n", v);
    printf("Control List Return Address 0: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_CT0LC];
    printf("[Control List Executor Thread 0 List Counter]: 0x%08"PRIx32"\n", v);
    printf("Major List Counter: %"PRIu32"\n", B(31,16));
    printf("Sub-list Counter: %"PRIu32"\n", B(15,0));
    printf("\n");

    v = peri[V3D_CT1LC];
    printf("[Control List Executor Thread 1 List Counter]: 0x%08"PRIx32"\n", v);
    printf("Major List Counter: %"PRIu32"\n", B(31,16));
    printf("Sub-list Counter: %"PRIu32"\n", B(15,0));
    printf("\n");

    v = peri[V3D_CT0PC];
    printf("[Control List Executor Thread 0 Primitive List Counter]: 0x%08"PRIx32"\n", v);
    printf("Primitive List Counter: %"PRIu32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_CT0PC];
    printf("[Control List Executor Thread 1 Primitive List Counter]: 0x%08"PRIx32"\n", v);
    printf("Primitive List Counter: %"PRIu32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_PCS];
    printf("[V3D Pipeline Control and Status]: 0x%08"PRIx32"\n", v);
    printf("Binning Mode Out Of Memory: %"PRIu32"\n", B(8,8));
    printf("Rendering Mode Busy: %"PRIu32"\n", B(3,3));
    printf("Rendering Mode Active: %"PRIu32"\n", B(2,2));
    printf("Binning Mode Busy: %"PRIu32"\n", B(1,1));
    printf("Binning Mode Active: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_BFC];
    printf("[Binning Mode Flush Count]: 0x%08"PRIx32"\n", v);
    printf("Flush Count: %"PRIu32"\n", B(7,0));
    printf("\n");

    v = peri[V3D_RFC];
    printf("[Rendering Mode Frame Count]: 0x%08"PRIx32"\n", v);
    printf("Frame Count: %"PRIu32"\n", B(7,0));
    printf("\n");

    v = peri[V3D_BPCA];
    printf("[Current Address of Binning Memory Pool]: 0x%08"PRIx32"\n", v);
    printf("Current Pool Address: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_BPCS];
    printf("[Remaining Size of Binning Memory Pool]: 0x%08"PRIx32"\n", v);
    printf("Size of Pool Remaining: %"PRIu32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_BPOA];
    printf("[Address of Overspill Binning Memory Block]: 0x%08"PRIx32"\n", v);
    printf("Address of Overspill Memory Block for Binning: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_BPOS];
    printf("[Size of Overspill Binning Memory Block]: 0x%08"PRIx32"\n", v);
    printf("Size of Overspill Memory Block for Binning: %"PRIu32"\n", B(31, 0));
    printf("\n");

    v = peri[V3D_BXCF];
    printf("[Binner Debug]: 0x%08"PRIx32"\n", v);
    printf("Disable Clipping: %"PRIu32"\n", B(1,1));
    printf("Disable Forwarding in State Cache: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_SQRSV0];
    printf("[Reserve QPUs 0-7]: 0x%08"PRIx32"\n", v);
    printf("Reservation settings for QPU  0: 0x%1"PRIx32"\n", B(3,0));
    printf("Reservation settings for QPU  1: 0x%1"PRIx32"\n", B(7,4));
    printf("Reservation settings for QPU  2: 0x%1"PRIx32"\n", B(11,8));
    printf("Reservation settings for QPU  3: 0x%1"PRIx32"\n", B(15,12));
    printf("Reservation settings for QPU  4: 0x%1"PRIx32"\n", B(19,16));
    printf("Reservation settings for QPU  5: 0x%1"PRIx32"\n", B(23,20));
    printf("Reservation settings for QPU  6: 0x%1"PRIx32"\n", B(27,24));
    printf("Reservation settings for QPU  7: 0x%1"PRIx32"\n", B(31,28));
    printf("\n");

    v = peri[V3D_SQRSV1];
    printf("[Reserve QPUs 8-15]: 0x%08"PRIx32"\n", v);
    printf("Reservation settings for QPU  8: 0x%1"PRIx32"\n", B(3,0));
    printf("Reservation settings for QPU  9: 0x%1"PRIx32"\n", B(7,4));
    printf("Reservation settings for QPU 10: 0x%1"PRIx32"\n", B(11,8));
    printf("Reservation settings for QPU 11: 0x%1"PRIx32"\n", B(15,12));
    printf("Reservation settings for QPU 12: 0x%1"PRIx32"\n", B(19,16));
    printf("Reservation settings for QPU 13: 0x%1"PRIx32"\n", B(23,20));
    printf("Reservation settings for QPU 14: 0x%1"PRIx32"\n", B(27,24));
    printf("Reservation settings for QPU 15: 0x%1"PRIx32"\n", B(31,28));
    printf("\n");

    v = peri[V3D_SQCNTL];
    printf("[QPU Scheduler Control]: 0x%08"PRIx32"\n", v);
    printf("Coordinate Shader Scheduling Bypass Limit: %"PRIu32"\n", B(3,2));
    printf("Vertex Shader Scheduling Bypass Limit: %"PRIu32"\n", B(1,0));
    printf("\n");

    if (print_unknown) {
        v = peri[V3D_SQCSTAT];
        printf("[SQCSTAT]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    v = peri[V3D_SRQPC];
    printf("[QPU User Program Request Programm Adress]: 0x%08"PRIx32"\n", v);
    if (print_writeonly)
        printf("Program Address: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_SRQUA];
    printf("[QPU User Program Request Uniforms Address]: 0x%08"PRIx32"\n", v);
    printf("Uniforms Address: 0x%08"PRIx32"\n", B(31,0));
    printf("\n");

    v = peri[V3D_SRQUL];
    printf("[QPU User Program Request Uniforms Length]: 0x%08"PRIx32"\n", v);
    printf("Uniforms Length: %"PRIu32"\n", B(11,0));
    printf("\n");

    v = peri[V3D_SRQCS];
    printf("[QPU User Program Request Control and Status]: 0x%08"PRIx32"\n", v);
    printf("Count of user programs completed: %"PRIu32"\n", B(23,16));
    printf("Count of user programs requests made: %"PRIu32"\n", B(15,8));
    printf("Queue Error: %"PRIu32"\n", B(7,7));
    printf("Queue Length: %"PRIu32"\n", B(5,0));
    printf("\n");

    v = peri[V3D_VPACNTL];
    printf("[VPM Allocator Control]: 0x%08"PRIx32"\n", v);
    printf("Enable VPM allocation timeout: %"PRIu32"\n", B(13,13));
    printf("Enable VPM allocation limits: %"PRIu32"\n", B(12,12));
    printf("Binning VPM allocation timeout %"PRIu32"\n", B(11,9));
    printf("Rendering VPM allocation timeout %"PRIu32"\n", B(8,6));
    printf("Binning VPM allocation limit %"PRIu32"\n", B(5,3));
    printf("Rendering VPM allocation limit %"PRIu32"\n", B(2,0));
    printf("\n");

    v = peri[V3D_VPMBASE];
    printf("[VPM base (user) memory reservation]: 0x%08"PRIx32"\n", v);
    printf("VPM memory reserved for user programs: %"PRIu32"\n", B(4,0));
    printf("\n");

    v = peri[V3D_PCTRC];
    printf("[Performance Counter Clear]: 0x%08"PRIx32"\n", v);
    if (print_writeonly)
        printf("Performance Counter Clear Bits: 0x%02"PRIx32"\n", B(15,0));
    printf("\n");

    v = peri[V3D_PCTRE];
    printf("[Performance Counter Enables]: 0x%08"PRIx32"\n", v);
    printf("Performance Counter Enable Bits: 0x%04"PRIx32"\n", B(15,0));
    printf("\n");

    for (i = 0; i < 16; i ++) {
        const unsigned pctr_list[16] = {
            V3D_PCTR0,  V3D_PCTR1,  V3D_PCTR2,  V3D_PCTR3,
            V3D_PCTR4,  V3D_PCTR5,  V3D_PCTR6,  V3D_PCTR7,
            V3D_PCTR8,  V3D_PCTR9,  V3D_PCTR10, V3D_PCTR11,
            V3D_PCTR12, V3D_PCTR13, V3D_PCTR14, V3D_PCTR15
        };
        const unsigned pctrs_list[16] = {
            V3D_PCTRS0,  V3D_PCTRS1,  V3D_PCTRS2,  V3D_PCTRS3,
            V3D_PCTRS4,  V3D_PCTRS5,  V3D_PCTRS6,  V3D_PCTRS7,
            V3D_PCTRS8,  V3D_PCTRS9,  V3D_PCTRS10, V3D_PCTRS11,
            V3D_PCTRS12, V3D_PCTRS13, V3D_PCTRS14, V3D_PCTRS15
        };
        const unsigned pctr = pctr_list[i], pctrs = pctrs_list[i];

        v = peri[pctr];
        printf("[Performance Counts (n=%2d)]: 0x%08"PRIx32"\n", i, v);
        printf("Performance Count (n=%2d): %"PRIu32"\n", i, B(31,0));
        printf("\n");

        v = peri[pctrs];
        printf("[Performance Counter Mapping (n=%2d)]: 0x%08"PRIx32"\n", i, v);
        printf("Performance Counter Device Id (n=%2d): %"PRIu32"\n", i, B(4,0));
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBCFG];
        printf("[DBCFG]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBSCS];
        printf("[DBSCS]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBSCFG];
        printf("[DBSCFG]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBSSR];
        printf("[DBSSR]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBSDR0];
        printf("[DBSDR0]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBSDR1];
        printf("[DBSDR1]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBSDR2];
        printf("[DBSDR2]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBSDR3];
        printf("[DBSDR3]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQRUN];
        printf("[DBQRUN]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQHLT];
        printf("[DBQHLT]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQSTP];
        printf("[DBQSTP]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQITE];
        printf("[DBQITE]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQITC];
        printf("[DBQITC]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQGHC];
        printf("[DBQGHC]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQGHG];
        printf("[DBQGHG]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    if (print_unknown) {
        v = peri[V3D_DBQGHH];
        printf("[DBQGHH]: 0x%08"PRIx32"\n", v);
        printf("\n");
    }

    v = peri[V3D_DBGE];
    printf("[PSE Error Signals]: 0x%08"PRIx32"\n", v);
    printf("IPD2_FPDUSED: %"PRIu32"\n", B(20,20));
    printf("IPD2_VALID: %"PRIu32"\n", B(19,19));
    printf("MULIP2: %"PRIu32"\n", B(18,18));
    printf("MULIP1: %"PRIu32"\n", B(17,17));
    printf("MULIP0: %"PRIu32"\n", B(16,16));
    printf("Error B reading VPM: %"PRIu32"\n", B(2,2));
    printf("Error A reading VPM: %"PRIu32"\n", B(1,1));
    printf("\n");

    v = peri[V3D_FDBG0];
    printf("[FEP Overrun Error Signals]: 0x%08"PRIx32"\n", v);
    printf("EZREQ_FIFO_ORUN: %"PRIu32"\n", B(17,17));
    printf("EZVAL_FIFO_ORUN: %"PRIu32"\n", B(15,15));
    printf("DEPTHO_ORUN: %"PRIu32"\n", B(14,14));
    printf("DEPTHO_FIFO_ORUN: %"PRIu32"\n", B(13,13));
    printf("REFXY_FIFO_ORUN: %"PRIu32"\n", B(12,12));
    printf("ZCOEFF_FIFO_FULL: %"PRIu32"\n", B(11,11));
    printf("XYRELW_FIFO_ORUN: %"PRIu32"\n", B(10,10));
    printf("XYRELO_FIFO_ORUN: %"PRIu32"\n", B(7,7));
    printf("FIXZ_ORUN: %"PRIu32"\n", B(6,6));
    printf("XYFO_FIFO_ORUN: %"PRIu32"\n", B(5,5));
    printf("QBSZ_FIFO_ORUN: %"PRIu32"\n", B(4,4));
    printf("QBFR_FIFO_ORUN: %"PRIu32"\n", B(3,3));
    printf("XYRELZ_FIFO_FULL: %"PRIu32"\n", B(2,2));
    printf("WCOEFF_FIFO_FULL: %"PRIu32"\n", B(1,1));
    printf("\n");

    v = peri[V3D_FDBGB];
    printf("[FEP Interface Ready and Stall Signals, plus FEP Busy Signals]: 0x%08"PRIx32"\n", v);
    printf("XYFO_FIFO_OP_READY: %"PRIu32"\n", B(28,28));
    printf("QXYF_FIFO_OP_READY: %"PRIu32"\n", B(27,27));
    printf("RAST_BUSY: %"PRIu32"\n", B(26,26));
    printf("EZ_XY_READY: %"PRIu32"\n", B(25,25));
    printf("EZ_DATA_READY: %"PRIu32"\n", B(23,23));
    printf("ZRWPE_READY: %"PRIu32"\n", B(7,7));
    printf("ZRWPE_STALL: %"PRIu32"\n", B(6,6));
    printf("EDGES_CTRLID: %"PRIu32"\n", B(5,3));
    printf("EDGES_ISCTRL: %"PRIu32"\n", B(2,2));
    printf("EDGES_READY: %"PRIu32"\n", B(1,1));
    printf("EDGES_STALL: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_FDBGR];
    printf("[FEP Internal Ready Signals]: 0x%08"PRIx32"\n", v);
    printf("FIXZ_READY: %"PRIu32"\n", B(30,30));
    printf("RECIPW_READY: %"PRIu32"\n", B(28,28));
    printf("INTERPRW_READY: %"PRIu32"\n", B(27,27));
    printf("INTERPZ_READY: %"PRIu32"\n", B(24,24));
    printf("XYRELZ_FIFO_LAST: %"PRIu32"\n", B(23,23));
    printf("XYRELZ_FIFO_READY: %"PRIu32"\n", B(22,22));
    printf("XYNRM_LAST: %"PRIu32"\n", B(21,21));
    printf("XYNRM_READY: %"PRIu32"\n", B(20,20));
    printf("EZLIM_READY: %"PRIu32"\n", B(19,19));
    printf("DEPTHO_READY: %"PRIu32"\n", B(18,18));
    printf("RAST_LAST: %"PRIu32"\n", B(17,17));
    printf("RAST_READY: %"PRIu32"\n", B(16,16));
    printf("XYFO_FIFO_READY: %"PRIu32"\n", B(14,14));
    printf("ZO_FIFO_READY: %"PRIu32"\n", B(13,13));
    printf("XYRELO_FIFO_READY: %"PRIu32"\n", B(11,11));
    printf("WCOEFF_FIFO_READY: %"PRIu32"\n", B(7,7));
    printf("XYRELW_FIFO_READY: %"PRIu32"\n", B(6,6));
    printf("ZCOEFF_FIFO_READY: %"PRIu32"\n", B(5,5));
    printf("REFXY_FIFO_READY: %"PRIu32"\n", B(4,4));
    printf("DEPTHO_FIFO_READY: %"PRIu32"\n", B(3,3));
    printf("EZVAL_FIFO_READY: %"PRIu32"\n", B(2,2));
    printf("EZREQ_FIFO_READY: %"PRIu32"\n", B(1,1));
    printf("QXYF_FIFO_READY: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_FDBGS];
    printf("[FEP Internal Stall Input Signals]: 0x%08"PRIx32"\n", v);
    printf("ZO_FIFO_IP_STALL: %"PRIu32"\n", B(28,28));
    printf("RECIPW_IP_STALL: %"PRIu32"\n", B(25,25));
    printf("INTERPW_IP_STALL: %"PRIu32"\n", B(22,22));
    printf("XYRELZ_FIFO_IP_STALL: %"PRIu32"\n", B(18,18));
    printf("INTERPZ_IP_STALL: %"PRIu32"\n", B(17,17));
    printf("DEPTHO_FIFO_IP_STALL: %"PRIu32"\n", B(16,16));
    printf("EZLIM_IP_STALL: %"PRIu32"\n", B(15,15));
    printf("XYNRM_IP_STALL: %"PRIu32"\n", B(14,14));
    printf("EZREQ_FIFO_OP_VALID: %"PRIu32"\n", B(13,13));
    printf("QXYF_FIFO_OP_VALID: %"PRIu32"\n", B(12,12));
    printf("QXYF_FIFO_OP_LAST: %"PRIu32"\n", B(11,11));
    printf("QXYF_FIFO_OP1_DUMMY: %"PRIu32"\n", B(10,10));
    printf("QXYF_FIFO_OP1_LAST: %"PRIu32"\n", B(9,9));
    printf("QXYF_FIFO_OP1_VALID: %"PRIu32"\n", B(8,8));
    printf("EZTEST_ANYQVALID: %"PRIu32"\n", B(7,7));
    printf("EZTEST_ANYQF: %"PRIu32"\n", B(6,6));
    printf("EZTEST_QREADY: %"PRIu32"\n", B(5,5));
    printf("EZTEST_VLF_OKNOVALID: %"PRIu32"\n", B(4,4));
    printf("EZTEST_STALL: %"PRIu32"\n", B(3,3));
    printf("EZTEST_IP_VLFSTALL: %"PRIu32"\n", B(2,2));
    printf("EZTEST_IP_PRSTALL: %"PRIu32"\n", B(1,1));
    printf("EZTEST_IP_QSTALL: %"PRIu32"\n", B(0,0));
    printf("\n");

    v = peri[V3D_ERRSTAT];
    printf("[Miscellaneous Error Signals (VPM, VDW, VCD, VCM, L2C)]: 0x%08"PRIx32"\n", v);
    printf("L2C AXI Receive Fifo Overrun error: %"PRIu32"\n", B(15,15));
    printf("VCM error (binner): %"PRIu32"\n", B(14,14));
    printf("VCM error (renderer): %"PRIu32"\n", B(13,13));
    printf("VCM Idle: %"PRIu32"\n", B(12,12));
    printf("VCD error - FIFO pointers out of sync: %"PRIu32"\n", B(11,11));
    printf("VDW error - address overflows: %"PRIu32"\n", B(10,10));
    printf("VPM error - allocated size error: %"PRIu32"\n", B(9,9));
    printf("VPM error - free non-allocated: %"PRIu32"\n", B(8,8));
    printf("VPM error - write non-allocated: %"PRIu32"\n", B(7,7));
    printf("VPM error - read non-allocated: %"PRIu32"\n", B(6,6));
    printf("VPM error - read range: %"PRIu32"\n", B(5,5));
    printf("VPM error - write range: %"PRIu32"\n", B(4,4));
    printf("VPM Allocator error - renderer request greater than limit: %"PRIu32"\n", B(3,3));
    printf("VPM Allocator error - binner request greater than limit: %"PRIu32"\n", B(2,2));
    printf("VPM Allocator error - request too big: %"PRIu32"\n", B(1,1));
    printf("VPM Allocator error - allocating base while busy: %"PRIu32"\n", B(0,0));

    err = vc4regmap_finalize();
    if (err) {
        fprintf(stderr, "error: vc4regmap_finalize\n");
        /* Continue finalization... */
    }

    if (flag_disable_qpu) {
        err = mailbox_qpu_enable(fd, 0);
        if (err) {
            fprintf(stderr, "error: qpu_enable\n");
            (void) mailbox_close(fd);
            exit(EXIT_FAILURE);
        }
    }
    if (flag_enable_qpu || flag_disable_qpu) {
        err = mailbox_close(fd);
        if (err) {
            fprintf(stderr, "error: mailbox_close\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
