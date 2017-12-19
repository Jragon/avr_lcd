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
    rectangle rect = {0, 20, 0, 20};
    fill_rectangle(rect, BLUE);
}

