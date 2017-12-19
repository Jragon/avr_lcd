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


int main(void)
{
    init_lcd();
    
    while (1) 
    {
        int i;
       
        for (i = 0; i < 10; i++)
        {
            display_char(i + '0');    
            _delay_ms(100);
        }
    }
}

