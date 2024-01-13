#include "common.h"
#include "mini_uart.h"
#include "printf.h"
#include "utils.h"
#include "irq.h"
#include "timer.h"

void putc(void *p, char c)
{
    if (c == '\n')
    {
        uart_send('\r');
    }

    uart_send(c);
}
void kernel_main(void)
{
    uart_init();
    init_printf(0, putc);
    printf("\nRaspberry PI Bare Metal OS Initializing...\n");

    irq_init_vectors();
    enable_interrupt_controller();
    irq_enable();
    timer_init();
    
#if RPI_VERSION == 3
    printf("\tBoard: Raspberry PI 3\n");
#endif

#if RPI_VERSION == 4
    printf("\tBoard: Raspberry PI 4\n");
#endif

    printf("\nException Level : %d\n", get_el());

    printf("Sleeping for 200 ms...\n");
    sleep(200);

    printf("Sleeping for 2 s...\n");
    sleep(2000);

    printf("Sleeping for 5 s...\n");
    sleep(5000);
    printf("Done.\n");

    while (1)
    {
        //uart_send(uart_recv());
    }   
}