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
ISR(TIMER2_COMPA_vect)
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
    init_sound();

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
    rects[0] = createRect(display.width/2, display.height/2, 10, 10, 2, 1, 45, 0, 1, BLUE);
    
    // paddle
    rects[1] = createRect(display.width/2, display.height - 10, 50, 5, 0, 1, 0, 1, 1, RED);
    paddle = &rects[1];
}

void init_frame_timer(int fps)
{
    // timer 1 ctc mode
    TCCR2A = _BV(WGM21);
    
    // / by 1024 
    TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);
    
    // F_CPU/prescalar = 12000000/1024 = 11719 ticks per second
    // timer 2 is only 8 bit so the max is 256
    // this means that the min FPS is 45
    OCR2A = (11719/fps < 256) ? 11719/fps : 255;

    // enable interrupt flag
    TIMSK2 |= _BV(OCIE1A);
}
