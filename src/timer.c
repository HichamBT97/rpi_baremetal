#include "printf.h"
#include "peripherals/timer.h"
#include "peripherals/irq.h"
#include "peripherals/aux.h"

const u32 interval_1 = CLOCKHZ;
u32 cur_val_1 = 0;

const u32 interval_3 = CLOCKHZ / 4;
u32 cur_val_3 = 0;

void timer_init()
{
    cur_val_1 = REGS_TIMER->counter_lo;
    cur_val_1 += interval_1;
    REGS_TIMER->compare[1] = cur_val_1;

    cur_val_3 = REGS_TIMER->counter_lo;
    cur_val_3 += interval_3;
    REGS_TIMER->compare[3] = cur_val_3;
}

void handle_timer_1()
{
    /* Redefine a new interval */
    cur_val_1 += interval_1;
    REGS_TIMER->compare[1] = cur_val_1;

    /* W1C */
    REGS_TIMER->counter_status |= SYS_TIMER_IRQ_1;

    /* This message should apear every 1 second */
    printf("Timer 1 recieved.\n");
}

void handle_timer_3()
{
    /* Redefine a new interval */
    cur_val_3 += interval_3;
    REGS_TIMER->compare[3] = cur_val_3;

    /* W1C */
    REGS_TIMER->counter_status |= SYS_TIMER_IRQ_3;

    /* This message should apear every 4 times in second */
    printf("Timer 3 recieved.\n");
}

/* Function that will get the number of ticks since RPI start */
u64 timer_get_ticks()
{
    u32 hi = REGS_TIMER->counter_hi;
    u32 lo = REGS_TIMER->counter_lo;

    /* double check hi value didn't change after setting it */
    if (hi != REGS_TIMER->counter_hi)
    {
        hi = REGS_TIMER->counter_hi;
        lo = REGS_TIMER->counter_lo;
    }

    return ((hi << 32) | lo);
}

/**
 * Sleep will take a value to wait in ms. while each tick is a 1us.
*/
void sleep(u32 ms)
{
    u64 start = timer_get_ticks();

    while (timer_get_ticks() < (start + (ms * 1000)));
}