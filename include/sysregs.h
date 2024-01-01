#pragma once
// Registers description from : DDI0487Fc_armv8_arm.pdf
//D13.2.113

#define SCTLR_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN         (0 << 24)
#define SCTLR_I_CACHE_DISABLED          (0 << 12)
#define SCTLR_D_CACHE_DISABLED          (0 << 2)
#define SCTLR_MMU_DISABLED              (0 << 0)
#define SCTLR_MMU_ENABLED               (1 << 0)

#define SCTLR_VALUE_MMU_DISABLED (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

//D13.2.47
// HCR[RW:31] : Execution state control for lower Exception levels
// HCR[RW:31] = 1 means that execution state for EL1 is aarch64, for EL0 is determined by the value of PSTATE.nRW
#define HCR_RW                          (1 << 31)
#define HCR_VALUE                       HCR_RW

//D13.2.112

#define SCR_RESERVED                    (3 << 4)
#define SCR_RW                          (1 << 10) // if EL2 exist -> EL2 is AArch64 & EL2 controls EL1&0, if not then EL1
#define SCR_NS                          (1 << 0)  // Set Exception level lower that EL3 to non-Secure state, EL2-0 cannot access to secure memory
#define SCR_VALUE                       (SCR_RESERVED | SCR_RW | SCR_NS)

//C5.2.19 Saved program status

#define SPSR_MASK_ALL                   (7 << 6) // Mask interrupts FIQ, IRQ and SError
// in aarch64, 
//Bits[3,2] : choose EL we want to move to. 
//Bits[1] : not used for unused. 
//Bits[0] : Used to store the PSTATE.SP value on taking an exception to EL3, and copied back to it on executing an exception return in EL3
//PSTATE[SP] : Stack pointer register selection bit
//PSTATE[SP] = 0 or (t) indicates use of the SP_EL0 stack pointer for all ELx.
//PSTATE[SP] = 1 or (h) indicates use of the SP_ELx stack pointer for the ELx. EX. EL2 use SP_EL2
#define SPSR_EL1h                       (5 << 0) 
#define SPSR_EL2h                       (9 << 0)
#define SPSR_VALUE                      (SPSR_MASK_ALL | SPSR_EL1h)
