#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <time.h>

#include "verilated_vcd_c.h"

#include "qemu.h"
#include "reg.h"
#include "dut.h"

#define WAVE_TRACE

// dump qemu registers
void print_qemu_registers(qemu_regs_t *regs, bool wpc) {
    if (wpc) eprintf("$pc:  0x%016lx\n", regs->pc);
    eprintf("$zero:0x%016lx  $ra:0x%016lx  $sp: 0x%016lx  $gp: 0x%016lx\n",
            regs->gpr[0], regs->gpr[1], regs->gpr[2], regs->gpr[3]);
    eprintf("$tp:  0x%016lx  $t0:0x%016lx  $t1: 0x%016lx  $t2: 0x%016lx\n",
            regs->gpr[4], regs->gpr[5], regs->gpr[6], regs->gpr[7]);
    eprintf("$fp:  0x%016lx  $s1:0x%016lx  $a0: 0x%016lx  $a1: 0x%016lx\n",
            regs->gpr[8], regs->gpr[9], regs->gpr[10], regs->gpr[11]);
    eprintf("$a2:  0x%016lx  $a3:0x%016lx  $a4: 0x%016lx  $a5: 0x%016lx\n",
            regs->gpr[12], regs->gpr[13], regs->gpr[14], regs->gpr[15]);
    eprintf("$a6:  0x%016lx  $a7:0x%016lx  $s2: 0x%016lx  $s3: 0x%016lx\n",
            regs->gpr[16], regs->gpr[17], regs->gpr[18], regs->gpr[19]);
    eprintf("$s4:  0x%016lx  $s5:0x%016lx  $s6: 0x%016lx  $s7: 0x%016lx\n",
            regs->gpr[20], regs->gpr[21], regs->gpr[22], regs->gpr[23]);
    eprintf("$s8:  0x%016lx  $s9:0x%016lx  $s10:0x%016lx  $s11:0x%016lx\n",
            regs->gpr[24], regs->gpr[25], regs->gpr[26], regs->gpr[27]);
    eprintf("$t3:  0x%016lx  $t4:0x%016lx  $t5: 0x%016lx  $t6: 0x%016lx\n",
            regs->gpr[28], regs->gpr[29], regs->gpr[30], regs->gpr[31]);
}

bool inst_is_branch(Inst inst) {
    if (0x2 <= inst.op && inst.op <= 0x7) return true;
    if (0x14 <= inst.op && inst.op <= 0x17) return true;

    if (inst.op == 0x00) { // special table
        if (inst.func == 0x08 || inst.func == 0x9) return true;
        return false;
    }

    if (inst.op == 0x01) { // regimm table
        if (0x00 <= inst.rt && inst.rt <= 0x03) return true;
        if (0x10 <= inst.rt && inst.rt <= 0x13) return true;
        return false;
    }

    return false;
}

bool inst_is_load_mmio(Inst inst, qemu_regs_t *regs) {
    if (inst.op == 0x23) {
        uint32_t addr = regs->gpr[inst.rs] + inst.simm;
        return 0xA0000000 <= addr && addr < 0xC0000000;
    }
    return false;
}

bool inst_is_store_mmio(Inst inst, qemu_regs_t *regs) {
    if (inst.op == 0x2b) {
        uint32_t addr = regs->gpr[inst.rs] + inst.simm;
        return 0xA0000000 <= addr && addr < 0xC0000000;
    }
    return false;
}

bool inst_is_mmio(Inst inst, qemu_regs_t *regs) {
    return inst_is_load_mmio(inst, regs) ||
           inst_is_store_mmio(inst, regs);
}

void difftest_start_qemu(const char *path, int use_sbi, int port, int ppid) {
    // install a parent death signal in the child
    int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
    if (r == -1) { panic("prctl error"); }

    if (getppid() != ppid) { panic("parent has died"); }

    close(0); // close STDIN

    qemu_start(path, use_sbi, port);    // start qemu in single-step mode and stub gdb
}

// #define DiffAssert(cond, fmt, ...)                        \
//   do {                                                    \
//     if (!(cond)) {                                        \
//       nemu_epilogue();                                    \
//       eprintf("nemu: %s:%d: %s: Assertion `%s' failed\n", \
//           __FILE__, __LINE__, __func__, #cond);           \
//       eprintf("\e[1;31mmessage: " fmt "\e[0m\n",          \
//           ##__VA_ARGS__);                                 \
//       difftest_finish_qemu(conn);                         \
//     }                                                     \
//   } while (0)

void __attribute__((noinline))
difftest_finish_qemu(qemu_conn_t *conn) {
    for (int i = 0; i < 2; i++) {
        qemu_regs_t regs = {0};
        qemu_single_step(conn);
        qemu_getregs(conn, &regs);
        print_qemu_registers(&regs, true);
    }
    abort();
}

// bool in_sync_addr(diff_pcs *dut_pcs) {
//     static const int alen = 2;
//     static uint32_t mfc0_count_pcs[alen] = {0x800058ec, 0x80024b7c};
//     for (int i = 0; i < alen; i++) {
//         for (int j = 0; j < 3; j++) {
//             if (dut_pcs->mycpu_pcs[j] == mfc0_count_pcs[i]) return true;
//         }
//     }
//     return false;
// }

bool difftest_regs(qemu_regs_t *regs, qemu_regs_t *dut_regs, diff_pcs *dut_pcs) {
    const char *alias[32] = {"zero", "ra", "sp", "gp",
                             "tp", "t0", "t1", "t2",
                             "fp", "s1", "a0", "a1",
                             "a2", "a3", "a4", "a5",
                             "a6", "a7", "s2", "s3",
                             "s4", "s5", "s6", "s7",
                             "s8", "s9", "s10", "s11",
                             "t3", "t4", "t5", "t6"};
    // if (regs->pc - 4 != dut_pcs->mycpu_pcs[0] && regs->pc - 4 != dut_pcs->mycpu_pcs[1] && regs->pc - 4 != dut_pcs->mycpu_pcs[2]) {
    //   printf("  |  at DS [%x %x %x]", dut_pcs->mycpu_pcs[0], dut_pcs->mycpu_pcs[1], dut_pcs->mycpu_pcs[2]);
    // }
    static uint32_t last_3_qpcs[3] = {0};
    for (int i = 0; i < 32; i++) {
        if (regs->gpr[i] != dut_regs->gpr[i]) {
            // if (in_sync_addr(dut_pcs)) {
                // dut_sync_reg(i, regs->gpr[i], true);
            // } else {
                sleep(2);
                for (int j = 0; j < 3; j++) {
                    printf("QEMU PC at [0x%08x]\n", last_3_qpcs[j]);
                }
                printf("\x1B[31mError in $%s, QEMU %lx, ZJV2 %lx\x1B[37m\n", alias[i], regs->gpr[i], dut_regs->gpr[i]);
                return false;
            // }
        }
    }
    last_3_qpcs[0] = last_3_qpcs[1];
    last_3_qpcs[1] = last_3_qpcs[2];
    last_3_qpcs[2] = regs->pc - 4;
    return true;
}

char *get_wf_filename() {
    char *filename = new char[64];
    time_t now = time(0);
    strftime(filename, sizeof(filename), "%F", localtime(&now));
    strcat(filename, ".vcd"); 
    return filename;
}

bool check_end_ysyx() {
    return dut->io_difftest_finish;
}

bool check_and_close_difftest(qemu_conn_t *conn, VerilatedVcdC* vfp, VerilatedContext* context) {
    if (check_end_ysyx()) {
        printf("difftest pass!\n");
#ifdef WAVE_TRACE
        dut_step(100, vfp, context);
        vfp->close();
        delete vfp;
        delete context;
#endif
        qemu_disconnect(conn);
        return true;
    }
    return false;
}

int difftest_body(const char *path, int port) {
    int result = 0;
    Verilated::traceEverOn(true);
    VerilatedVcdC* vfp;
    VerilatedContext* contextp;
    dut = new VTileForVerilator;
    vfp = new VerilatedVcdC;
    contextp = new VerilatedContext;
#ifdef WAVE_TRACE
    dut->trace(vfp, 99);
    vfp->open("sim.vcd");
    // dut->dump(0);
#endif
    qemu_regs_t regs = {0};
    qemu_regs_t dut_regs = {0};
    diff_pcs dut_pcs = {0};
    int bubble_count = 0;
    uint64_t duts = 0;

    static int ugly_cnt = 0;
    static long bypass = 500000;

    qemu_conn_t *conn = qemu_connect(port);

    extern uint64_t elf_entry;
    regs.pc = elf_entry;
    qemu_break(conn, elf_entry);
    qemu_continue(conn);
    qemu_remove_breakpoint(conn, elf_entry);
    qemu_setregs(conn, &regs);
    qemu_getregs(conn, &regs);

    // set up device under test
    dut_reset(10, vfp, contextp);
    dut_sync_reg(0, 0, false);

    // for(int i = 0; i < 100; i++) {
    //     dut_step(1, vfp, contextp);
    // }
    // vfp->flush();
    // vfp->close();
    // delete vfp;
    // delete contextp;
    // qemu_disconnect(conn);
    // printf("end\n");
    // return;

    while (1) {
        dut_step(1, vfp, contextp);
        if (check_and_close_difftest(conn, vfp, contextp)) return 0;
        bubble_count = 0;
        dut_sync_reg(0, 0, false);

        while (dut_commit() == 0) {
            dut_step(1, vfp, contextp);
            if (check_and_close_difftest(conn, vfp, contextp)) 
                return 0;

            bubble_count++;
            // printf("dut bubble count: %d\n", bubble_count);

            if (bubble_count > 10) {
                printf("Too many bubbles.\n");
                break;
            }
        }


        // if (bc > 2048 * 8) {
            // break;
        // }
        for (int i = 0; i < dut_commit(); i++) {
            qemu_single_step(conn);

#ifdef TRACE
            printf("\nQEMU\n");
            print_qemu_registers(&regs, true);
            printf("\nDUT\n");
            for (int i = 0; i < 3; i++) {
                printf("$pc_%d:0x%016lx  ", i, dut_pcs.mycpu_pcs[i]);
            }
            printf("\n");
            print_qemu_registers(&dut_regs, false);
            printf("==============\n");
#endif
        }

        qemu_getregs(conn, &regs);
        dut_getregs(&dut_regs);
        dut_getpcs(&dut_pcs);

        if (!difftest_regs(&regs, &dut_regs, &dut_pcs)) {
            sleep(1);
            printf("\nQEMU\n");
            print_qemu_registers(&regs, true);
            printf("\nDUT\n");
            for (int i = 0; i < 3; i++) {
                printf("$pc_%d:0x%016lx  ", i, dut_pcs.mycpu_pcs[i]);
            }
            printf("\n");
            print_qemu_registers(&dut_regs, false);
            printf("\n");
            result = 1;
            break;
        }
    }

#ifdef WAVE_TRACE
    dut_step(3, vfp, contextp);
    vfp->close();
    delete vfp;
    delete contextp;
#endif
    qemu_disconnect(conn);

    return result;
}

int difftest(const char *path, int use_sbi) {
    int port = 1234;
    int ppid = getpid();
    int result = 0;

    printf("Welcome to ZJV2 differential test with QEMU!\n");

    if (fork() != 0) {    // child process
        result = difftest_body(path, port);
    } else {              // parent process
        difftest_start_qemu(path, use_sbi, port, ppid);
    }

    return result;
}
