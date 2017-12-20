#pragma once
#include <avr/io.h>

#define TONE_PRESCALER 8
#define SPEAKER_PIN PD4
#define SPEAKER_ON DDRD |= _BV(SPEAKER_PIN)
#define SPEAKER_OFF DDRD &= ~_BV(SPEAKER_PIN)

// noises
#define BEEP 880
#define BOOP 294

void init_sound(void);
void tone(uint16_t frequency);
void play(uint16_t frequency, uint16_t delay);