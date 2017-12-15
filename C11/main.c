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

#define NEW_RECT(x, y, width, height) { x, x + width, y, y + height }

int8_t enc_delta(void);
void init_rotary(void);
volatile int8_t delta;

ISR( TIMER0_COMPA_vect ) {
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
         delta += (diff & 2) - 1;   /* bit 1 = direction (+/-) */
     }

}

ISR(TIMER1_COMPA_vect)
{
    const int inc = 5;
    static movingRectangle rects[3] = {
        {
            .rect = NEW_RECT(STARTX, STARTY, WIDTH, HEIGHT),
            .oldRect = { 0 }, // empty as there is no old rect
            .width = WIDTH, .height = HEIGHT,
            .direction = 1, .theta = 45, .intercept = 0,
            .fixed = 0, .hittable = 1, // not fixed, and collides
            .collision = -1, .colour = BLUE
        },
		{
            .rect = NEW_RECT(100, 100, WIDTH, HEIGHT),
            .oldRect = { 0 }, // empty as there is no old rect
            .width = WIDTH, .height = HEIGHT,
            .direction = 1, .theta = 30, .intercept = 100,
            .fixed = 0, .hittable = 1, // not fixed, and collides
            .collision = -1, .colour = RED
        },
        {
            { LCDWIDTH/2, LCDWIDTH/2 + 20, 300, 310 }, { 0 },
            // note that tan(0) returns 0
            1, 0, 300, 20, 10, 
            1, 1, // fixed and collides
            -1, RED
        }
    };
    
    
    testCollisions(rects, 3);
    
    if (!moveRectangle(&rects[0], inc) || !moveRectangle(&rects[1], inc)){
		display.x = display.width/2;
		display.y = display.height/2;
		
		clear_screen();
		display_string("GAME OVER");
		
		_delay_ms(10000);
		
		//clear_screen();
		//printMovingRectangle(&rects[0]);
		//redrawRectangles(rects, 2);
	} else {
		
		moveRectangle(&rects[2], enc_delta()*2);
    
		//printMovingRectangle(&rect1);
		redrawRectangles(rects, 3);
	}
}


int main(void)
{
    init_lcd();
    
    /* note that uart takes a lot of time
       and will slow down will increase the time
       takes to run the ISR */
    init_debug_uart0();
	
	init_rotary();
    
    // init timer1
    // timer 1 ctc mode
    TCCR1A = 0;
    TCCR1B = _BV(WGM12);
    // / by 1024
    TCCR1B |= _BV(CS12) | _BV(CS10);
    
    // ~30Hz when clock at 12MHz
    // OCR1A = 390;
	OCR1A = 1000;
	
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


/* read two step encoder */
int8_t enc_delta() {
    int8_t val;

    cli();
    val = delta;
    delta &= 1;
    sei();

    return val >> 1;
}
