#include "utils.h"
#include "gpio.h"

void gpio_pin_set_func(u8 piNumber,GpioFunc func)
{
    /* Based on the pin number we got the start bit offset on the func register*/
    u8 bitStart = (piNumber * 3) % 30;

    /* Each func Reg manipulate 10 pins, reg will take the register number 0 to 5 based on pin number*/
    u8 reg = piNumber / 10;

    /* Read the equivalent func select register based on pin number */
    u32 selector = REGS_GPIO->func_select[reg];

    /* zeroise the equivalent 3 bit for the pin number */
    selector &= ~(7 << bitStart);

    /* Set The func value that we want */
    selector |= (func << bitStart);

    /* Write back to the func register */
    REGS_GPIO->func_select[reg] = selector;
}

void gpio_pin_enable(u8 piNumber)
{
    /* Write to GPPUD to disable */
    REGS_GPIO->pupd_enable = 0;

    /* Wait 150 cycles */
    delay(150);

    /* Wrtie to specific clock in GPPUDCLK0/1 */
    REGS_GPIO->pupd_enable_clock[piNumber / 32] = 1 << (piNumber % 32);

    /* Wait 150 cycles */
    delay(150);

    /* Wrtie again to GPPUD & GPPUDCLK0/1 to remove the control signal */
    REGS_GPIO->pupd_enable = 0;
    REGS_GPIO->pupd_enable_clock[piNumber / 32] = 0;
}