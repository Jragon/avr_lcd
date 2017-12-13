#define F_CPU 12000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "lcd.h"
#include "debug.h"
#include "movement.h"

#define WIDTH 10
#define HEIGHT 10
#define STARTX (LCDWIDTH/2 - WIDTH/2)
#define STARTY (LCDHEIGHT/2 - HEIGHT/2)


ISR(TIMER1_COMPA_vect)
{
    const int inc = 5;
    static movingRectangle rect1 = {
        { STARTX, STARTX + WIDTH, STARTY, STARTY + HEIGHT },
        1, 45, 0, WIDTH, HEIGHT
    };
    
    rectangle oldRect = rect1.rect;
    
    moveRectangle(&rect1, inc);
    printMovingRectangle(&rect1);
    
    fill_rectangle(oldRect, display.background);
    fill_rectangle(rect1.rect, BLUE);
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
    OCR1A = 50;
    
    // enable interrupt flag
    TIMSK1 |= _BV(OCIE1A);
    
    /* Rotary encoder setup */
    DDRD |= _BV(PD2) | _BV(PD3);
    PORTD |= _BV(PD2) | _BV(PD3);
    sei();

    display.background = BLACK;
    //set_orientation(South);
    while (1);
}

