/*
 * C11.c
 *
 * Created: 12/12/2017 16:29:50
 * Author : RoryWork
 */ 

#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "debug.h"

void setXY(int x, int y)
{
	display.x = x;
	display.y = y;
}

int main(void)
{
    init_lcd();
    init_debug_uart0();

    display.x = LCDWIDTH/2;
    display.y = LCDHEIGHT/2;
    display_char('N');

    rectangle rect1 = { 0, 20, 0, 20 };
    rectangle rect2 = { 10, 30, 10, 30 };

    fill_rectangle(rect1, BLUE);
    fill_rectangle(rect2, RED);

}

void testBoundaryCollsion(movingRectangle *rect)
{
    // collision with boundaries, only if it doesn't collide with something else
    // x axis collision
    if ((rect->right >= display.width) || (rect->left <= 0)) {
        direction = -direction;
        theta = -theta;
        intercept = rect->top - rect->left*tan(_toRadians(theta));
    }
                
    // y axis collision
    // no direction change
    if ((rect->bottom >= display.height) || (rect->top <= 0)) {
        theta = -theta;
        intercept = rect->top - rect->left*tan(_toRadians(rects->theta));
    }
}