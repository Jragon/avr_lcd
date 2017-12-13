/*
 * C11.c
 *
 * Created: 12/12/2017 16:29:50
 * Author : RoryWork
 */ 

#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "debug.h"

int main(void)
{
    init_lcd();
    init_debug_uart0();
    
    int i, j;
    for (j = 0; j < 10; j++)
    {
        for (i = 0; i < 40; i++)
        {
            printf("row: %d, col: %d, x: %d, y: %d\n", j, i, display.x, display.y);
            
            display_char('z');
            _delay_ms(10);
        }
    }
}

