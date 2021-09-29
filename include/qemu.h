#ifndef QEMU_H
#define QEMU_H

#include <stdbool.h>
#include "gdb_proto.h"
#include "isa.h"

typedef struct gdb_conn qemu_conn_t;

int qemu_start(const char *elf, int use_sbi, int port);

qemu_conn_t *qemu_connect(int port);

void qemu_disconnect(qemu_conn_t *conn);

// bool qemu_memcpy_to_qemu_small(qemu_conn_t *conn, uint32_t dest, void *src, int len);

// bool qemu_memcpy_to_qemu(qemu_conn_t *conn, uint32_t dest, void *src, int len);

bool qemu_getregs(qemu_conn_t *conn, qemu_regs_t *r);

bool qemu_setregs(qemu_conn_t *conn, qemu_regs_t *r);

bool qemu_single_step(qemu_conn_t *conn);

void qemu_break(qemu_conn_t *conn, uint64_t entry);

void qemu_remove_breakpoint(qemu_conn_t *conn, uint64_t entry);

void qemu_continue(qemu_conn_t *conn);

inst_t qemu_getinst(qemu_conn_t *conn, uint32_t pc);

#endif
