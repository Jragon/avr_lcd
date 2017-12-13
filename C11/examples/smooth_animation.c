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

ISR(TIMER1_COMPA_vect)
{
    const int inc = 5;
    static rectangle rect = { _startX, _startX + _width, _startY, _startY + _height };

    // 1 = left, -1 = right
    static int direction = 1;
    static int theta = 68;
    static int intercept = 0;
    
    int right, left, top, bottom;

    
    right = rect.right + direction*inc;
    left = rect.left + direction*inc;
    
    top = tan(_toRadians(theta))*left + intercept;
    bottom = top + _width;
    
    // hit right boundary
    if (right >= 240) {
        right = 240;
        direction = -direction;
        theta = -theta;
        intercept = top - LCDWIDTH/tan(_toRadians(theta));
    }
    
    // hit bottom boundary
    if (bottom >= 320) {
        bottom = 320;
        theta = -theta;
        intercept = LCDHEIGHT - (left*tan(_toRadians(theta))) - _height;
    }
    
    // hit left boundary
    if (left <= 0){
        left = 0;
        direction = -direction;
        theta = -theta;
        intercept = top;
    }
    
    // hit top boundary
    if (top <= 0){
        top = 0;
        theta = -theta;
        intercept = -left*tan(_toRadians(theta));
    }
    
    
    
    printf("left: %d, right: %d, top: %d, bottom: %d, theta: %d, dir: %d, int: %d\n",
           left, right, top, bottom, theta, direction, intercept);
    printf("%d, %d, %d", display.background, BLACK, BLUE);
    fill_rectangle(rect, display.background);

    rect.left = left;
    rect.right = right;
    rect.top = top;
    rect. bottom = bottom;

    //clear_screen();    
    fill_rectangle(rect, BLUE);
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
    sei();

    display.background = BLACK;
    
    while (1);
}

