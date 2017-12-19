#define F_CPU 12000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "debug.h"
#include "movement.h"

#define WIDTH 10
#define HEIGHT 10
#define STARTX (LCDWIDTH/2 - WIDTH/2)
#define STARTY (LCDHEIGHT/2 - HEIGHT/2)
#define RECT_COUNT 2

#define NEW_RECT(x, y, width, height) { x, x + width, y, y + height }

int8_t encodeRotaryCount(void);
void init_rotary(void);
void init_rectangles(void);
void init_frame_timer(int fps);

volatile int8_t rotaryCount;
// note that you have cast it to (movingRectangle) when you pass it
volatile movingRectangle rects[RECT_COUNT];
volatile movingRectangle *paddle;

// rotary encoder timer
ISR(TIMER0_COMPA_vect)
{
    static int8_t last;
    int8_t new, diff;
    uint8_t wheel;

    wheel = PIND;
    new = 0;
    if( wheel  & _BV(PD2) ) new = 3;
    if( wheel  & _BV(PD3) )
    new ^= 1;                  /* convert gray to binary */
    diff = last - new;         /* difference last - new  */
    if( diff & 1 ){            /* bit 0 = value (1) */
        last = new;                /* store new as next last  */
        rotaryCount += (diff & 2) - 1;   /* bit 1 = direction (+/-) */
    }
}

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

void init_rotary(void) {

    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    /* Configure I/O Ports */
    DDRD &= ~_BV(PD2) & ~_BV(PD3);  /* Rot. Encoder inputs */
    PORTD |= _BV(PD2) | _BV(PD3);   /* Rot. Encoder pull-ups */


    /* Timer 0 for switch scan interrupt: */
    TCCR0A = _BV(WGM01);  /* CTC Mode, DS Table 14-7 */
    TCCR0B = _BV(CS01)
           | _BV(CS00);   /* Prescaler: F_CPU / 64 */
    OCR0A = (uint8_t)(F_CPU / (2 * 64.0 * 1000) - 1);

    TIMSK0 |= _BV(OCIE0A); 
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

/* read two step encoder */
int8_t encodeRotaryCount() {
    int8_t val;

    cli();
    val = rotaryCount;
    rotaryCount &= 1;
    sei();

    return val >> 1;
}
