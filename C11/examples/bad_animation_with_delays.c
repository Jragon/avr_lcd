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
    
    // 10x10 rect at (0, 0)
    rectangle rect;

    while (1)
    {
        int i;
        int startX = 0, startY = 0;
        int width = 10, height = 10;
        
        rect.left = startX;
        rect.right = width;
        rect.top = startY;
        rect.bottom = height;
        
        for (i = 0; i < 50; i += 5)
        {
            rect.left = startX + i;
            rect.right = width + i;
            rect.top = startY + i;
            rect.bottom = height + i;
            
            clear_screen();
            fill_rectangle(rect, BLUE);

            _delay_ms(4);
        }
    }
}

