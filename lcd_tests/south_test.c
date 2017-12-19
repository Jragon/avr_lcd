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

void setXY(int x, int y)
{
	display.x = x;
	display.y = y;
}

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
	set_orientation(South);
    
    display.x = LCDWIDTH/2;
    display.y = LCDHEIGHT/2;
    display_char('S');


	for (j = 0; j < 4; ++j)
	{
	    fill_rectangle(rect[j], BLUE);
	    _delay_ms(500);
	}

	setXY(0, 0);
	display_string("0, 0");

	setXY(0, LCDHEIGHT - 6);
	display_string("0, HEIGHT");
}
