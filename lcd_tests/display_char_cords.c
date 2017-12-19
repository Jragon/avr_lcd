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
    
    
	display_string("Hello World");
	while(1);
}

