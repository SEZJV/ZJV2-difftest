#!/bin/bash

CYAN='\033[0;36m'
NC='\033[0m' # No Color

main() {
    # difftest return code
    dt_ret_code=0
    
    make -j
    test_dir=riscv-tests
    for FILE in cases/$test_dir/*; do
        elf=${FILE:6}
        amo="amo"
        if [[ "$elf" =~ "amo" ]]; then
            echo -e "Start testing on ${CYAN}${elf}${NC} !!!"

            make prepare ELF=/$elf
            pushd build
            ./emulator
        
            ret_code=$?
            if [ $ret_code != 0 ]; then
                dt_ret_code=1
            fi    
        
            popd
        fi
    done 
    return $dt_ret_code
}

main
