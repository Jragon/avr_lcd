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
    
    // left and right are the x coords
    // top and bottom are the y coords
    // col is colour, not column

    int size = 20;

    rectangle rect[4] = {
    	{0, size, 0, size},
    	{LCDWIDTH - size, LCDWIDTH, 0, size},
    	{0, size, LCDHEIGHT - size, LCDHEIGHT},
    	{LCDWIDTH - size, LCDWIDTH, LCDHEIGHT - size, LCDHEIGHT}
    };

    int i, j;
    while(1){
    	for (i = 0; i < 4; ++i)
    	{
    		set_orientation(i);

    		display.x = LCDWIDTH / 2;
    		display.y = LCDHEIGHT / 2;
    		display_char(i + '0');

    		printf("%d\n", i);

    		for (j = 0; j < 4; ++j)
    		{
			    fill_rectangle(rect[j], BLUE);
			    _delay_ms(500);
    		}

    		_delay_ms(2000);

    		for (j = 0; j < 4; ++j)
    		{
			    fill_rectangle(rect[j], BLACK);
			    _delay_ms(10);
    		}
    		_delay_ms(300);
    	}
    }
}

