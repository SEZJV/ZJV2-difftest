#!/bin/bash


CYAN='\033[0;36m'
NC='\033[0m' # No Color

main() {
    make
    for FILE in cases/benchmark/*; do
        elf=${FILE:6:-4}

        echo -e "Start testing on ${CYAN}${elf}${NC} !!!"

        make prepare ELF=/$elf
        pushd build
        ./emulator -elf
        
        ret_code=$?
        if [ $ret_code != 0 ]; then
            exit $ret_code
        fi    
        
        popd
    done 
}

main
