#ifndef QEMU_H
#define QEMU_H

#include "gdb_proto.h"
#include <stdbool.h>

typedef struct gdb_conn qemu_conn_t;
typedef union {
  struct {
	uint32_t zero, at, v0, v1, a0, a1, a2, a3;
	uint32_t t0, t1, t2, t3, t4, t5, t6, t7;
	uint32_t s0, s1, s2, s3, s4, s5, s6, s7;
	uint32_t t8, t9, k0, k1, gp, sp, s8, ra;
	uint32_t sr, lo, hi, bad, cause, pc;
	uint32_t fsr, fir;
  };
  struct { uint32_t array[40]; };
  struct { uint32_t gpr[32]; };
} qemu_regs_t;

int qemu_start(const char *elf, int port);
qemu_conn_t *qemu_connect(int port);
void qemu_disconnect(qemu_conn_t *conn);

bool qemu_memcpy_to_qemu_small(
    qemu_conn_t *conn, uint32_t dest, void *src, int len);
bool qemu_memcpy_to_qemu(
    qemu_conn_t *conn, uint32_t dest, void *src, int len);
bool qemu_getregs(qemu_conn_t *conn, qemu_regs_t *r);
bool qemu_setregs(qemu_conn_t *conn, qemu_regs_t *r);
bool qemu_single_step(qemu_conn_t *conn);
void qemu_break(qemu_conn_t *conn, uint32_t entry);
void qemu_remove_breakpoint(
    qemu_conn_t *conn, uint32_t entry);
void qemu_continue(qemu_conn_t *conn);

#endif
