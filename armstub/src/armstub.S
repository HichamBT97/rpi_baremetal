.globl _start
_start:
    ldr w0, kernel_entry32 // load location of kernel_entry32
    br x0                  // Branch to it 

.ltorg                     // Create some organization

.org 0xf0                  // Specific address where it should the variable stub_magic
.globl stub_magic          // FW clears the next 8 Bytes after reading initial value
stub_magic:
    .word 0x5afe570b       // stub_magic = 0x5afe570b  

.org 0xf4
.globl stub_version
stub_version:
    .word 0

.org 0xfc                  // This address will store the kernel_entry32 variable
.globl kernel_entry32      // Which is the start address of the kernel
kernel_entry32:
    .word 0x0              // The kernel start address is in the top of the binary file