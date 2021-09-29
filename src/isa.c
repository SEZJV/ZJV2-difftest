#include "isa.h"

int inst_is_load_uart(inst_t inst) {
    if (inst.i_inst_t.opcode == 0x3) {
        return 1;
    } else {
        return 0;
    }
}
