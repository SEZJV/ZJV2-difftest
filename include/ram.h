#ifndef __RAM_H
#define __RAM_H

#include "common.h"
#include "qemu.h"

#ifndef EMU_RAM_SIZE
#define EMU_RAM_SIZE (256 * 1024 * 1024UL)
#endif

// void init_ram(const char *img);
void init_ram(const char *img, qemu_conn_t *conn);
void ram_finish();
void* get_ram_start();
long get_ram_size();

void* get_img_start();
long get_img_size();

uint64_t pmem_read(uint64_t raddr);
void pmem_write(uint64_t waddr, uint64_t wdata);

#ifdef WITH_DRAMSIM3
#include "axi4.h"

void dramsim3_finish();
void dramsim3_helper_rising(const struct axi_channel &axi);
void dramsim3_helper_falling(struct axi_channel &axi);
#endif

#endif