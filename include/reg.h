#ifndef CPU_H
#define CPU_H

#include "common.h"

#define CPU_INIT_PC 0xbfc00000

#define CP0_RESERVED_BASE 0 // for segment
#define CP0_RESERVED_SERIAL 1
#define CP0_RESERVED_CHECK 2
#define CP0_RESERVED_PRINT_REGISTERS 3
#define CP0_RESERVED_PRINT_INSTR_QUEUE 4
#define CP0_RESERVED_TOGGLE_COMMITS 5
#define CP0_RESERVED_HIT_TRAP 6

#define CP0_INDEX 0
#define CP0_RANDOM 1
#define CP0_ENTRY_LO0 2
#define CP0_ENTRY_LO1 3
#define CP0_CONTEXT 4 // maintained by kernel
#define CP0_PAGEMASK 5
#define CP0_WIRED 6
#define CP0_RESERVED 7 // for extra debug and segment
#define CP0_BADVADDR 8
#define CP0_COUNT 9
#define CP0_ENTRY_HI 10
#define CP0_COMPARE 11
#define CP0_STATUS 12
#define CP0_CAUSE 13
#define CP0_EPC 14
#define CP0_PRID 15  // sel = 0
#define CP0_EBASE 15 // sel = 1
#define CP0_CONFIG 16
#define CP0_ErrorEPC 30

#define CP0_PRID_SEL 0  // sel = 0
#define CP0_EBASE_SEL 1 // sel = 1

#define CP0_TAG_LO 28
#define CP0_TAG_HI 29

#define CU0_ENABLE 1
#define CU1_ENABLE 2
#define CU2_ENABLE 4
#define CU3_ENABLE 8

#define CPR_VAL(cpr) *((uint32_t *)(void *)&(cpr))

/*
 * default config:
 *   {1'b1, 21'b0, 3'b1, 4'b0, cp0_regs_Config[2:0]};
 * //Release 1
 *
 * default config1:
 *   {1'b0, 6'd15, 3'd1, 3'd5, 3'd0, 3'd2, 3'd5, 3'd0,
 * 7'd0};
 *
 *   Cache Size:   I:128-64B-direct, D:256-64B-direct
 */

typedef struct {
  uint32_t IE : 1;
  uint32_t EXL : 1;
  uint32_t ERL : 1;
  uint32_t R0 : 1;

  uint32_t UM : 1;
  uint32_t UX : 1;
  uint32_t SX : 1;
  uint32_t KX : 1;

  uint32_t IM : 8;

  uint32_t Impl : 2;
  uint32_t : 1;
  uint32_t NMI : 1;
  uint32_t SR : 1;
  uint32_t TS : 1;

  uint32_t BEV : 1;
  uint32_t PX : 1;

  uint32_t MX : 1;
  uint32_t RE : 1;
  uint32_t FR : 1;
  uint32_t RP : 1;
  uint32_t CU : 4;
} cp0_status_t;

typedef struct {
  uint32_t : 2;
  uint32_t ExcCode : 5;
  uint32_t : 1;
  uint32_t IP : 8;

  uint32_t : 6;
  uint32_t WP : 1;
  uint32_t IV : 1;

  uint32_t : 2;
  uint32_t PCI : 1;
  uint32_t DC : 1;
  uint32_t CE : 2;
  uint32_t TI : 1;
  uint32_t BD : 1;
} cp0_cause_t;

typedef struct {
  uint32_t revision : 8;
  uint32_t processor_id : 8;
  uint32_t company_id : 8;
  uint32_t company_options : 8;
} cp0_prid_t;

typedef struct {
  uint32_t K0 : 3; // kseg0 coherency algorithms
  uint32_t : 4;    // must be zero
  uint32_t MT : 3; // MMU type
                   // 0 for none
                   // 1 for standard TLB
                   // 2 xxx, 3 xxx
  uint32_t AR : 3; // 0 for revision 1
  uint32_t AT : 2; // 0 for mips32,
                   // 1 for mips64 with access only to
                   // 32-bit seg 2 for mips64 with all
                   // access to 32-bit seg 3 reserved
  uint32_t BE : 1; // 0 for little endian, 1 for big endian
  uint32_t Impl : 15;
  uint32_t M : 1; // donate that config1 impled at sel 1
} cp0_config_t;

// 1'b0, 6'd15, 3'd1, 3'd5, 3'd0, 3'd2, 3'd5, 3'd0, 7'd0
typedef struct {
  uint32_t FP : 1; // FPU present bit
  uint32_t EP : 1; // EJTAG present bit
  uint32_t CA : 1; // code compression present bit
  uint32_t WR : 1; // watch registers present bit

  uint32_t PC : 1; // performance counter present bit
  uint32_t MD : 1; // not used on mips32 processor
  uint32_t C2 : 1; // coprocessor present bit

  uint32_t DA : 3; // dcache associativity
                   // 0 for direct mapped
                   // 2^(DA) ways
                   // ---------------------------
  uint32_t DL : 3; // dcache line size:
                   // 0 for no icache, 7 reserved
                   // othwise: 2^(DL + 1) bytes
                   // ---------------------------
  uint32_t DS : 3; // dcache sets per way:
                   // 2^(IS + 8)
                   // ---------------------------
  uint32_t IA : 3; // icache associativity
                   // 0 for direct mapped
                   // 2^(IA) ways
                   // ---------------------------
  uint32_t IL : 3; // icache line size:
                   // 0 for no icache, 7 reserved
                   // othwise: 2^(IL + 1) bytes
                   // ---------------------------
  uint32_t IS : 3; // icache sets per way:
                   // 2^(IS + 8)
                   // ---------------------------
  uint32_t
      MMU_size : 6; // 0 to 63 indicates 1 to 64 TLB entries
  uint32_t M : 1;   // indicate config 2 is present
} cp0_config1_t;

typedef struct {
  uint32_t : 13;
  uint32_t mask : 16;
  uint32_t : 3;
} cp0_pagemask_t;

#define PABITS 32

// only 4KB page is supported
typedef struct {
  uint32_t asid : 8;
  uint32_t : 5;
  uint32_t vpn : 19;
} cp0_entry_hi_t;

typedef struct {
  uint32_t g : 1;
  uint32_t v : 1;
  uint32_t d : 1;
  uint32_t c : 3;
  /* uint32_t pfn : 24; */
  uint32_t pfn : (PABITS - 12);
  uint32_t : (36 - PABITS);
  uint32_t : 2;
} cp0_entry_lo_t;

typedef struct {
  uint32_t : 13;
  uint32_t mask : 16;
  uint32_t : 3;
} cp0_page_mask_t;

typedef uint32_t cp0_wired_t;

#define TLB_BITS 5
#define NR_TLB_ENTRY (1 << TLB_BITS)

typedef struct {
  uint32_t : 4;
  uint32_t BadVPN2 : 19;
  uint32_t PTEBase : 9;
} cp0_context_t;

typedef struct {
  uint32_t idx : TLB_BITS;
  uint32_t : (32 - 1 - TLB_BITS);
  uint32_t p : 1;
} cp0_index_t;

typedef union {
  uint32_t cpr[32][8];

#define __ glue(__, __LINE__)
  /* clang-format off */
  struct {
	struct { cp0_index_t index;         uint32_t __[7]; };
	struct { uint32_t random;           uint32_t __[7]; };
	struct { cp0_entry_lo_t entry_lo0;  uint32_t __[7]; };
	struct { cp0_entry_lo_t entry_lo1;  uint32_t __[7]; };
	struct { cp0_context_t context;     uint32_t __[7]; };
	struct { cp0_pagemask_t pagemask;   uint32_t __[7]; };
	struct { cp0_wired_t wired;         uint32_t __[7]; };
	uint32_t reserved[8];                 /* reserved */
	struct { uint32_t badvaddr;         uint32_t __[7]; };
	struct { uint32_t count[2];         uint32_t __[6]; };
	struct { cp0_entry_hi_t entry_hi;   uint32_t __[7]; };
	struct { uint32_t compare;          uint32_t __[7]; };
	struct { cp0_status_t status;       uint32_t __[7]; };
	struct { cp0_cause_t cause;         uint32_t __[7]; };
	struct { vaddr_t epc;               uint32_t __[7]; };
	struct { cp0_prid_t prid;
	         vaddr_t ebase;             uint32_t __[6]; };
	struct { cp0_config_t config;
	         cp0_config1_t config1;     uint32_t __[6]; };
  };
#undef __
  /* clang-format on */
} cp0_t;

/* clang-format off */
enum {
  R_zero, R_at, R_v0, R_v1,
  R_a0, R_a1, R_a2, R_a3,
  R_t0, R_t1, R_t2, R_t3,
  R_t4, R_t5, R_t6, R_t7,
  R_s0, R_s1, R_s2, R_s3,
  R_s4, R_s5, R_s6, R_s7,
  R_t8, R_t9, R_k0, R_k1,
  R_gp, R_sp, R_fp, R_ra,
};
/* clang-format on */

/* ref:http://www.jaist.ac.jp/iscenter-new/mpc/old-machines/altix3700/opt/toolworks/totalview.6.3.0-1/doc/html/ref_guide/MIPSFCSRRegister.html,
 * shit MIPS_Volume 3 */
typedef union {
  struct {
    uint32_t RM : 2;      /* bit 0..1, rounding mode
                           * 0: round to nearest
                           * 1: round toward zero
                           * 2: round toward positive infinity
                           * 3: round toward negative infinity */
    uint32_t flags : 5;   /* bit 2..6, flags
                           * 1 << 0, inexact result
                           * 1 << 1, underflow
                           * 1 << 2, overflow
                           * 1 << 3, divide by zero
                           * 1 << 4, invalid operation */
    uint32_t enables : 5; /* bit 7..11, enables
                           * 1 << 0, inexact result
                           * 1 << 1, underflow
                           * 1 << 2, overflow
                           * 1 << 3, divide by zero
                           * 1 << 4, invalid operation */
    uint32_t causes : 6;  /* bit 12..17, causes
                           * 1 << 0, inexact result
                           * 1 << 1, underflow
                           * 1 << 2, overflow
                           * 1 << 3, divide by zero
                           * 1 << 4, invalid operation
                           * 1 << 5, unimplemented */
    uint32_t unused : 5;  /* bit 18..22 */
    uint32_t fcc0 : 1;    /* bit 23, FPU condition code 0 */
    uint32_t fs : 1;      /* bit 24, flush to zero */
    uint32_t fcc1_7 : 7;  /* bit 25..31, FPU condition
                             code 1..7 */
  };
  uint32_t val;
} fcsr_t;

typedef struct {
  uint32_t gpr[32];
  uint32_t hi, lo;
  cp0_t cp0;
  vaddr_t pc;

  vaddr_t br_target;
#if CONFIG_DELAYSLOT
  bool is_delayslot;
#endif
  bool has_exception;
#if CONFIG_DUMP_SYSCALL
  bool is_syscall;
#endif

  union {
    uint32_t fpr32i[32];
    float fpr32f[32];
    uint64_t fpr64i[16];
    double fpr64f[16];
  };
  fcsr_t fcsr;
} CPU_state;

static inline bool getFPCondCode(uint8_t cc) {
  extern CPU_state cpu;
  if (cc == 0)
    return cpu.fcsr.fcc0;
  else if (cc < 8)
    return (cpu.fcsr.fcc1_7 >> (cc - 1)) & 1;
  abort();
}

static inline void setFPCondCode(uint8_t cc, bool v) {
  extern CPU_state cpu;
  if (cc == 0)
    cpu.fcsr.fcc0 = v;
  else if (cc < 8) {
    uint8_t mask = ~(1 << (cc - 1));
    cpu.fcsr.fcc1_7 =
        (cpu.fcsr.fcc1_7 & mask) | (v << (cc - 1));
  }
}

#define CAUSE_IP_TIMER 0x80

#define EXC_INTR 0    /* interrupt */
#define EXC_TLBM 1    /* tlb modification */
#define EXC_TLBL 2    /* tlb load */
#define EXC_TLBS 3    /* tlb store */
#define EXC_AdEL 4    /* exception on load */
#define EXC_AdES 5    /* exception on store */
#define EXC_IBE 6     /* instruction bus error */
#define EXC_DBE 7     /* data bus error */
#define EXC_SYSCALL 8 /* syscall */
#define EXC_BP 9      /* breakpoint */
#define EXC_RI 10     /* reserved instruction */
#define EXC_CPU 11    /* ????? */
#define EXC_OV 12     /* arithmetic overflow */
#define EXC_TRAP 13   /* trap */

#define EX_EJTAG_DEBUG 1
#define EX_RESET 2
#define EX_SOFT_RESET 3
#define EX_NMI 4
#define EX_MACHINE_CHECK 5
#define EX_ADDR_ERROR 6
#define EX_TLB_REFILL 7
#define EX_TLB_INVALID 8
#define EX_TLB_MODIFIED 9
#define EX_CACHE_ERROR 10
#define EX_BUS_ERROR 11
#define EX_OVERFLOW 12
#define EX_TRAP 13
#define EX_SYSCALL 14
#define EX_BREAK 15
#define EX_RI 16
#define EX_CO_UNUSABLE 17
#define EX_FP 18
#define EX_WATCH 19
#define EX_INTR 20

#define MAKE_EX(EX, CODE) (((EX) << 16) | CODE)

enum {
  FPU_FMT_S = 16,
  FPU_FMT_D = 17,
  FPU_FMT_W = 20,
  FPU_FMT_L = 21,
  FPU_FMT_PS,
  FPU_FMT_OB,
  FPU_FMT_QH,
  FPU_FMT_UW,
  FPU_FMT_UD,
};

typedef struct {
  union {
    uint32_t val;
    // R-type
    struct {
      uint32_t func : 6;
      uint32_t shamt : 5;
      uint32_t rd : 5;
      uint32_t rt : 5;
      uint32_t rs : 5;
      uint32_t op : 6;
    };

    uint32_t uimm : 16; // I-type

    int32_t simm : 16; // SI-type

    uint32_t addr : 26; // J-type
    uint32_t sel : 3;   // MFC0

    // FPU
    struct {
      uint32_t : 6;
      uint32_t fd : 5;
      uint32_t fs : 5;
      uint32_t ft : 5;
      uint32_t fmt : 5;
      uint32_t : 6;
    };

    struct {
      uint32_t : 6;
      uint32_t : 1;
      uint32_t fd64 : 4;
      uint32_t : 1;
      uint32_t fs64 : 4;
      uint32_t : 1;
      uint32_t ft64 : 4;
      uint32_t : 5;
      uint32_t : 6;
    };

    struct {
      uint32_t cond : 4; // bit 0..3
      uint32_t fc : 2;   // bit 4..5
      uint32_t A0 : 1;   // bit 6
      uint32_t : 1;      // bit 7
      uint32_t cc1 : 3;  // bit 8..10
    };

    struct {
      uint32_t : 16;    // bit 0..15
      uint32_t tf : 1;  // bit 16
      uint32_t nd : 1;  // bit 17
      uint32_t cc2 : 3; // bit 18..20
      uint32_t bc : 5;  // bit 21..25
    };
  };
} Inst; // Instruction

extern CPU_state cpu;
int init_cpu(vaddr_t entry);
void nemu_set_irq(int irqno, bool val);

#endif
