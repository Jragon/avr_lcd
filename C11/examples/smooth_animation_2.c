#define F_CPU 12000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "lcd.h"
#include "debug.h"

#define _width 10
#define _height 10
#define _startX (LCDWIDTH/2 - _width/2)
#define _startY (LCDHEIGHT/2 - _height/2)
#define _toRadians(deg) (deg * (M_PI / 180.0))

typedef struct {
    rectangle rect;
    // signed ints
    int direction, theta, intercept;
} movingRectangle;

ISR(TIMER1_COMPA_vect)
{
    const int inc = 5;
    static movingRectangle rect1 = { 
        (rectangle) { _startX, _startX + _width, _startY, _startY + _height },
        1, 45, 0
    };
    
    int right, left, top, bottom;

    
    right = rect1.rect.right + rect1.direction*inc;
    left = rect1.rect.left + rect1.direction*inc;
    
    top = tan(_toRadians(theta))*left + rect1.intercept;
    bottom = top + _width;
    
    // hit right boundary
    if (right >= 240) {
        right = 240;
        rect1.direction = -rect1.direction;
        rect1.theta = -rect1.theta;
        rect1.intercept = top - LCDWIDTH/tan(_toRadians(theta));
    }
    
    // hit bottom boundary
    if (bottom >= 320) {
        bottom = 320;
        rect1.theta = -rect1.theta;
        rect1.intercept = LCDHEIGHT - (left*tan(_toRadians(theta))) - _height;
    }
    
    // hit left boundary
    if (left <= 0){
        left = 0;
        rect1.direction = -rect1.direction;
        rect1.theta = -rect1.theta;
        rect1.intercept = top;
    }
    
    // hit top boundary
    if (top <= 0){
        top = 0;
        rect1.theta = -rect1.theta;
        rect1.intercept = -left*tan(_toRadians(theta));
    }
    
    
    
    printf("left: %d, right: %d, top: %d, bottom: %d, theta: %d, dir: %d, int: %d\n",
           left, right, top, bottom, rect1.theta, rect1.direction, rect1.intercept);
    printf("%d, %d, %d", display.background, BLACK, BLUE);
    fill_rectangle(rect1.rect, display.background);

    rect1.rect.left = left;
    rect1.rect.right = right;
    rect1.rect.top = top;
    rect1.rect. bottom = bottom;

    //clear_screen();    
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
    OCR1A = 3;
    
    // enable interrupt flag
    TIMSK1 |= _BV(OCIE1A);
    
    /* Rotary encoder setup */
    DDRD |= _BV(PD2) | _BV(PD3);
    PORTD |= _BV(PD2) | _BV(PD3);
    sei();

    display.background = BLACK;
    
    while (1);
}

