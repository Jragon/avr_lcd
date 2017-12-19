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

    int i, j;
	set_orientation(South);
    
    display.x = LCDWIDTH/2;
    display.y = LCDHEIGHT/2;
    display_char('S');


    fill_rectangle((rectangle) { 0, 10, 0, 10 }, BLUE);
    fill_rectangle((rectangle) { 10, 20, 0, 20 }, RED);

    fill_rectangle((rectangle) { LCDWIDTH-10, LCDWIDTH, 0, 10 }, YELLOW);
	fill_rectangle((rectangle) { LCDWIDTH-20, LCDWIDTH-10, 0, 20 }, GREEN);

}
