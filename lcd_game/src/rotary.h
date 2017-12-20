#include <avr/io.h>
#include <avr/interrupt.h>

volatile int8_t rotaryCount;

ISR(TIMER0_COMPA_vect);
void init_rotary(void);
int8_t encodeRotaryCount();