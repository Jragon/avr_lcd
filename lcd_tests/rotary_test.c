/* COMP2215 15/16: Task 01---MODEL ANSWER */

/* For La Fortuna board

   | Port | Pin | Function                  |
   |------+-----+---------------------------|
   | B    |   7 | Green LED                 |
   | E    |   4 | Rotary Encoder A          |
   | E    |   5 | Rotary Encoder B          |

*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "debug.h"

#define STEP_DELAY_MS 5
#define MIN_STEP    2
#define MAX_STEP  255

void init(void);

int8_t enc_delta(void);
volatile int8_t delta;

/* Configure I/O Ports */
void init_rotary(void) {

    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    /* Configure I/O Ports */

    DDRB  |=  _BV(PB7);   /* LED pin out */
    PORTB &= ~_BV(PB7);   /* LED off */


    DDRD &= ~_BV(PD2) & ~_BV(PD3);  /* Rot. Encoder inputs */
    PORTD |= _BV(PD2) | _BV(PD3);   /* Rot. Encoder pull-ups */


    /* Timer 0 for switch scan interrupt: */

    TCCR0A = _BV(WGM01);  /* CTC Mode, DS Table 14-7 */
    TCCR0B = _BV(CS01)
           | _BV(CS00);   /* Prescaler: F_CPU / 64 */
    OCR0A = (uint8_t)(F_CPU / (2 * 64.0 * 1000) - 1);

    TIMSK0 |= _BV(OCIE0A); 
}



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

/* read two step encoder */
int8_t enc_delta() {
    int8_t val;

    cli();
    val = delta;
    delta &= 1;
    sei();

    return val >> 1;
}



int main(void) {
	init_debug_uart0();
    init_rotary();
    sei();
    for (;;) {
        _delay_ms(1000);
		printf("%d\n", enc_delta());
		
        PINB |= _BV(PINB7);
    }
}

