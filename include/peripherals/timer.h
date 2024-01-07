#pragma once

#include "base.h"
#include "common.h"

#define CLOCKHZ 1000000

// 12.1
struct timer_regs
{
    reg32 counter_status;
    reg32 counter_lo;
    reg32 counter_hi;
    reg32 compare[4];
};

#define REGS_TIMER ((struct timer_regs *)(PBASE + 0x3000))