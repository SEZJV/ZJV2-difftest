# ZJV2-difftest

ZJV2's difftest framework based on Verilator, QEMU and GDB.


## Requirement

### riscv-gnu-toolchain

`riscv-gnu-toolchain` has official release now!

```bash
$ wget https://github.com/riscv/riscv-gnu-toolchain/releases/download/2021.08.11/riscv64-elf-ubuntu-20.04-nightly-2021.08.11-nightly.tar.gz
$ tar -zxvf riscv64-elf-ubuntu-20.04-nightly-2021.08.11-nightly.tar.gz
$ sudo mv riscv /tools/riscv-elf
```



### verilator

Get the source code.

```bash
$ wget https://github.com/verilator/verilator/archive/refs/tags/v4.210.tar.gz
$ tar -zxvf v4.210.tar.gz
```

Configure and build

```bash
$ cd verilator-4.210
$ unset VERILATOR_ROOT
$ autoconf
$ ./configure
$ make -j8
$ sudo make install
```


### qemu-system-riscv64

Install dependencies.

```bash
$ sudo apt install wget ninja-build libglib2.0-dev libpixman-1-dev pkg-config
```

Get the source code.

```bash
$ wget https://download.qemu.org/qemu-6.0.0.tar.xz
$ tar xvJf qemu-6.0.0.tar.xz
```

Configure and build

```bash
$ cd qemu-6.0.0
$ ./configure --target-list=riscv32-softmmu,riscv64-softmmu
$ make -j
$ sudo make install
```


## Usage

`ZJV2-difftest` has been imported as a Git submodule of `ZJV2`. All the stuff regarding difftest can be fetched from `ZJV2` repo.


```bash
$ cd ZJV2-difftest
# prepare test cases
$ make prepare ELF=<test-elf-program>
$ make
$ cd build && ./emulator -elf cases/<test-elf-program>
```

The following is a simple use case:

```bash
$ cd ZJV2-difftest
$ make prepare ELF=rv64mi-p-access
$ make
$ cd build && ./emulator -elf cases/rv64mi-p-access
```
