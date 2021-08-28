#include "common.h"

extern void difftest();

const char *symbol_file = "../elf/ucore-kernel-initrd";
vaddr_t elf_entry = 0x80000000;

int main() {
    difftest();
}