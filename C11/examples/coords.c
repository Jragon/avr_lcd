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
    
    // at x = 0, y = 0. ie, the beginning of the screen
    char coords[20];
    sprintf(coords, "x: %d, y: %d ", display.x, display.height);
    display_string(coords);
    
    // move the cursor to the middle
    //display.x = display.width / 2;
    //display.y = display.height / 2;
    //display_string("Hello Again!");
    
    int i;
    for (i = 0; i < 100; i++)
    {
        sprintf(coords, "x: %d, y: %d ", display.x, display.height);
        display_string(coords);
    }
    
    while (1);
}

