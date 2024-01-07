#include "gpio.h"
#include "utils.h"
#include "peripherals/aux.h"
#include "mini_uart.h"

#define TXD 14
#define RXD 15

#define MU_EN_REC_IRQ

void uart_init()
{
    gpio_pin_set_func(TXD, GFAlt5);
    gpio_pin_set_func(RXD, GFAlt5);

    gpio_pin_enable(TXD);
    gpio_pin_enable(RXD);

    /* Enable mini uart */
    REGS_AUX->enables = 1;

    /* Disable TX/RX */
    REGS_AUX->mu_control = 0;
    
    /* Disable interrupts */
#ifndef MU_EN_REC_IRQ
    REGS_AUX->mu_ier = 0;
#else
    /* Enable receive interrupt */
#if RPI_VERSION == 3
    REGS_AUX->mu_ier = 0xD; //BCM2837
#endif

#if RPI_VERSION == 4
    REGS_AUX->mu_ier = 0xE; //BCM2711
#endif
#endif
    /* UART works ni 8-bits format */
    REGS_AUX->mu_lcr = 3;

    /* RTS line is High - RTS : Request to send */
    REGS_AUX->mu_mcr = 0;

    /* Use different BDRATE based on RPI version */
    /* Formula : BDRATE = (SYS_CLK_FRQ / (8*(BR_REG + 1)) */
    /* Or      : BR_REG = (SYS_CLK_FRQ / (8*BDRATE)) - 1 */
#if RPI_VERSION == 3
    /* BDRATE = 115200 bits/s @ SYS_CLK_FRQ = 400MHz --> BR_REG = 433 */
    /* BDRATE = 9600 bits/s  @ SYS_CLK_FRQ = 400MHz--> BR_REG = 5207 */
    REGS_AUX->mu_baud_rate = 433;
#endif

#if RPI_VERSION == 4
    /* BDRATE = 115200 bits/s @ SYS_CLK_FRQ = 500MHz --> BR_REG = 541 */
    /* BDRATE = 9600 bits/s  @ SYS_CLK_FRQ = 500MHz--> BR_REG = 6509 */
    REGS_AUX->mu_baud_rate = 541;
#endif

    /* Enable TX/RX */
    REGS_AUX->mu_control = 3;

    uart_send('\r');
    uart_send('\n');
    uart_send('\n');

}
char uart_recv()
{
    /* Wait till RX FIFO can accept at least one byte */
    while (!(REGS_AUX->mu_lsr & 0x1));

    /* Data is taken for RX FIFO */
    return (REGS_AUX->mu_io & 0xFF);
}
void uart_send(char c)
{
    /* Wait till TX FIFO can accept at least one byte */
    while (!(REGS_AUX->mu_lsr & 0x20));

    /* data written is put in the transmit FIFO*/
    REGS_AUX->mu_io = c;
}

void uart_send_string(char *str)
{
    while (*str)
    {
        if (*str == '\n')
        {
            uart_send('\r');
        }

        uart_send(*str);
        str++;
    }
}