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
    static movingRectangle rects[2] = {
        { 
            { STARTX, STARTX + WIDTH, STARTY, STARTY + HEIGHT },
            { 0 }, // empty as there is no old rect
            1, 17, 0, WIDTH, HEIGHT, BLUE
        },
        {
            { 0, 50, 0, 10 }, { 0 },
            // note that tan(0) returns 0
            1, 0, 0, WIDTH, HEIGHT, RED
        }
    };
    
    moveRectangle(&rects[0], inc);
    moveRectangle(&rects[1], 1);
    
    //printMovingRectangle(&rect1);
    redrawRectangles(rects, 2);
}

int main(void)
{
    init_lcd();
    
    /* note that uart takes a lot of time
       and will slow down will increase the time
       takes to run the ISR */
    init_debug_uart0();
    
    // init timer1
    // timer 1 ctc mode
    TCCR1A = 0;
    TCCR1B = _BV(WGM12);
    // / by 1024
    TCCR1B |= _BV(CS12) | _BV(CS10);
    
    // ~30Hz when clock at 12MHz
    OCR1A = 390;
    
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

