#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "debug.h"
#include "movement.h"
#include "rotary.h"

#define WIDTH 10
#define HEIGHT 10
#define STARTX (LCDWIDTH/2 - WIDTH/2)
#define STARTY (LCDHEIGHT/2 - HEIGHT/2)
#define RECT_COUNT 2

#define NEW_RECT(x, y, width, height) { x, x + width, y, y + height }

void init_rectangles(void);
void init_frame_timer(int fps);

// note that you have cast it to (movingRectangle) when you pass it
volatile movingRectangle rects[RECT_COUNT];
volatile movingRectangle *paddle;

// frame timer
ISR(TIMER1_COMPA_vect)
{
    testCollisions((movingRectangle *) rects, RECT_COUNT);
    
    if (paddle != NULL)
        // read value from rotary encoder count
        paddle->increment = encodeRotaryCount() * 2;

    moveRectangles((movingRectangle *) rects, RECT_COUNT);
    redrawRectangles((movingRectangle *) rects, RECT_COUNT);
}

int main(void)
{
    init_lcd();
    
    /* note that uart takes a lot of time
       and will slow down will increase the time
       takes to run the ISR */
    init_debug_uart0();
	
	init_rotary();
    init_rectangles();
    init_frame_timer(30);

    sei();

    display.background = BLACK;
    //set_orientation(South);
    while (1);
}

void init_rectangles(void) {
    rects[0] = createRect(display.width/2, display.height/2, 10, 10, 5, 1, 45, 0, 1, BLUE);
    
    // paddle
    rects[1] = createRect(display.width/2, display.height - 10, 50, 5, 0, 1, 0, 1, 1, RED);
    paddle = &rects[1];
}

void init_frame_timer(int fps)
{
    // timer 1 ctc mode
    TCCR1A = 0;
    TCCR1B = _BV(WGM12);
    
    // / by 1024
    TCCR1B |= _BV(CS12) | _BV(CS10);
    
    OCR1A = 11719/fps;
        
    // enable interrupt flag
    TIMSK1 |= _BV(OCIE1A);
}
