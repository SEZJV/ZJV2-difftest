TARGET_DIR  := $(CURDIR)/build
INCLUDE_DIR := $(CURDIR)/include
SRC_DIR     := $(CURDIR)/src
OBJ_DIR     := $(TARGET_DIR)/obj
VSRC_DIR    := $(CURDIR)/verilog

CROSS_COMPILE := riscv64-unknown-elf-

SRC         := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.cpp)
OBJ         := $(patsubst $(SRC_DIR)/%.c $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

CC          := gcc
CFLAGS      := -I$(INCLUDE_DIR) -O3

VERILATOR_VSRC_DIR	:=	./../build/verilog/base
VERILATOR_CSRC_DIR	:=	$(CURDIR)/src
VERILATOR_DEST_DIR	:=	$(TARGET_DIR)/verilator
VERILATOR_CXXFLAGS	:=	-O3 -std=c++11 -fpermissive -g -I$(VERILATOR_CSRC_DIR) -I$(VERILATOR_DEST_DIR)/build -I$(INCLUDE_DIR)
VERILATOR_LDFLAGS 	:=	-Wl,--export-dynamic -lpthread -ldl
VERILATOR_SOURCE 	:=  $(sort $(wildcard $(VERILATOR_CSRC_DIR)/*.cpp)) $(sort $(wildcard $(VERILATOR_CSRC_DIR)/*.c))

VERILATOR_FLAGS := --cc --exe --trace --top-module TileForVerilator	\
				  --threads 8 \
				  --assert --x-assign unique    \
				  --output-split 20000 -O3    	\
				  -I$(VERILATOR_VSRC_DIR) 	  	\
				  -CFLAGS "$(VERILATOR_CXXFLAGS)" \
				  -LDFLAGS "$(VERILATOR_LDFLAGS)"

CASES_DIR	:= $(CURDIR)/cases

all: $(TARGET_DIR)/emulator

$(TARGET_DIR)/emulator: $(SRC)
	mkdir -p build
	cp -v $(VERILATOR_VSRC_DIR)/TileForVerilator.v $(TARGET_DIR)/TileForVerilator.v
	mkdir -p $(VERILATOR_DEST_DIR)
	verilator $(VERILATOR_FLAGS) -o $(TARGET_DIR)/emulator -Mdir $(VERILATOR_DEST_DIR)/build $(TARGET_DIR)/TileForVerilator.v $(VERILATOR_SOURCE)
	$(MAKE) -C $(VERILATOR_DEST_DIR)/build -f $(VERILATOR_DEST_DIR)/build/VTileForVerilator.mk

prepare:
	mkdir -p build
	cp -v $(CASES_DIR)/$(ELF) $(TARGET_DIR)/testfile.elf
	$(CROSS_COMPILE)objdump -d $(TARGET_DIR)/testfile.elf > $(TARGET_DIR)/testfile.dump
	$(CROSS_COMPILE)objcopy -O binary $(TARGET_DIR)/testfile.elf $(TARGET_DIR)/testfile.bin
	$(CROSS_COMPILE)objdump -d $(TARGET_DIR)/testfile.elf > $(TARGET_DIR)/testfile.dump
	od -t x1 -An -w1 -v $(TARGET_DIR)/testfile.bin > $(TARGET_DIR)/testfile.hex


clean:
	-@rm -rf $(TARGET_DIR)
