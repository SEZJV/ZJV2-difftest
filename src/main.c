#include <stdio.h>
#include <string.h>

#include "common.h"
#include "difftest.h"


// const char *symbol_file = "../elf/ucore-kernel-initrd";
uint64_t elf_entry = 0x80000000;


void help() {
    printf("ZJV2-difftest Usage\n\n");
    printf("./emulator -os  <path/to/os>\tdifftest an os.\n");
    printf("./emulator -elf <path/to/elf>\tdifftest an elf.\n");
    printf("./emulator -help             \tdisplay this brief manual.");
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("too few arguments\n\n");
        help();
        return 1;
    }

    if (!strcmp(argv[1], "-help")) {
        help();
    } else if (!strcmp(argv[1], "-os") || !strcmp(argv[1], "-elf")) {
        if (argc < 3) {
            printf("too few arguments\n\n");
            help();
            return 1;
        }
        difftest(argv[2], !strcmp(argv[1], "-os"));
    } else {
        printf("invalid option\n\n");
        help();
        return 1;
    }

    return 0;
}