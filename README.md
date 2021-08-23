# ZJV2-difftest

ZJV2's difftest framework based on Verilator, QEMU and GDB.


## Requirement

### riscv-gnu-toolchain

TODO

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

### Get ZJV2-difftest source code

```bash
$ git clone https://github.com/PAN-Ziyue/ZJV2-difftest
```


### Clean

```bash
$ make clean
```
