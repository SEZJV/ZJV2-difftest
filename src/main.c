#include <stdio.h>
#include <string.h>

#include "common.h"
#include "difftest.h"


uint64_t elf_entry = 0x80000000;


void help() {
    printf("ZJV2-difftest Usage\n\n");
    printf("./emulator -os  \tdifftest an os.\n");
    printf("./emulator -elf \tdifftest an elf.\n");
    printf("./emulator -help\tdisplay this brief manual.");
}


int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("too few arguments\n\n");
        help();
        return 1;
    }

    if (!strcmp(argv[1], "-help")) {
        help();
    } else if (!strcmp(argv[1], "-os") || !strcmp(argv[1], "-elf")) {
        difftest("testfile.elf", !strcmp(argv[1], "-os"));
    } else {
        printf("invalid option\n\n");
        help();
        return 1;
    }

    return 0;
}