#include "mm.h"
#include "sysregs.h"

.section ".text.boot"

.globl _start
_start:
    mrs x0, mpidr_el1 // Get affinity register : mention if a multiprocessor system
    and x0, x0, #0xFF // Save Aff0, if Aff0 = 0, then it is a multiproc sys.
    cbz x0, master    // Branch to master if x0 = 0
    b proc_hang

master:
    // Process to moving to EL1 for kernel
    ldr x0, =SCTLR_VALUE_MMU_DISABLED // Disable MMU...
    msr sctlr_el1, x0

    ldr x0, =HCR_VALUE                // Execution state for EL1 is AArch64
    msr hcr_el2, x0

    ldr x0, =SCR_VALUE                // Set Exception level lower than EL3 to non-secure mode & specify EL2/1/0 as AArch64
    msr scr_el3, x0

    ldr x0, =SPSR_VALUE
    msr spsr_el3, x0

    adr x0, el1_entry
    msr elr_el3, x0

    eret

el1_entry:
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    bl memzero

    mov sp, #LOW_MEMORY
    bl kernel_main
    b proc_hang

proc_hang:
    wfe
    b proc_hang