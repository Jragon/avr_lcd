#include "sound.h"

ISR(TIMER1_OVF_vect)
{
    if (delayCounter > delayCounterTop)
    {
        SPEAKER_OFF;

        // disable TIMER1 interrupt
        TIMSK1 &= ~_BV(TOIE1);
    }
    else
    {
        delayCounter++;
    }
}

void init_sound(void)
{
    // Fast PWM mode with ICR1 as top
    TCCR1A = _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12);

    // Clear OC1B on Compare Match, set OC1B at bottom
    TCCR1A |= _BV(COM1B1);

    // prescalar / by 8
    TCCR1B |= _BV(CS11);

    // ICR1 sets the freq, and OCR1B sets the duty
    // Freq = (F_CPU / PRESCALAR) / ICR1,
    // Duty width (seconds) = 1 / [(F_CPU / PRESCALAR) / OCR1B]

    // this means that OCR1B is effectively the volume
    // ie, the higher the duty the higher the volume

    // ICR should be set by the tone function
    ICR1 = 0;
    OCR1B = 256;
}

void tone(uint16_t frequency)
{
    // set frequency
    ICR1 = (uint16_t)((F_CPU / (2 * TONE_PRESCALER)) / frequency);
}

void play(uint16_t frequency, uint16_t delay_ms)
{
    SPEAKER_ON;

    // set up delay counter
    delayCounter = 0;
    delayCounterTop = ((double)(1000.0 / frequency)) * delay_ms;

    // enable interrupt
    TIMSK1 = _BV(TOIE1);

    tone(frequency);
}