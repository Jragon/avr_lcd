/*
 * C11.c
 *
 * Created: 12/12/2017 16:29:50
 * Author : RoryWork
 */ 

#define F_CPU 12000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"
#include "debug.h"

#define _width 10
#define _height 10
#define _startX (LCDWIDTH/2 - _width/2)
#define _startY (LCDHEIGHT/2 - _height/2)

ISR(TIMER1_COMPA_vect)
{
    const int inc = 5;
    static rectangle rect = { _startX, _startX + _width, _startY, _startY + _height };

    
    if (rect.right <= 240) {
        rect.right += inc;
        rect.left += inc;
    } else {
        rect.right = _startX + _width;
        rect.left = _startX;
    }
    
    printf("left: %d, right: %d, top: %d, bottom: %d\n", 
           rect.left, rect.right, rect.top, rect.bottom);

    clear_screen();
    fill_rectangle(rect, BLUE);    
}   


int main(void)
{
    init_lcd();
    init_debug_uart0();
    
    // init timer1
    // timer 1 ctc mode
    TCCR1A = 0;
    TCCR1B = _BV(WGM12);
    // / by 1024
    TCCR1B |= _BV(CS12) | _BV(CS10);
    	
    // ~30Hz when clock at 12MHz
    OCR1A = 385;
    	
    // enable interrupt flag
    TIMSK1 |= _BV(OCIE1A);
    sei();

    while (1);
}

