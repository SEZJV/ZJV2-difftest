#include <iostream>
#include <vector>
#include <string>

#include "common.h"
#include "difftest.h"

uint64_t elf_entry = 0x80000000;


int main() {
    int result = difftest("testfile.elf");

    return result;
}