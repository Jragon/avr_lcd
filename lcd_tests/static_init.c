#define F_CPU 12000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "lcd.h"
#include "debug.h"

#define WIDTH 10
#define HEIGHT 10
#define STARTX (LCDWIDTH/2 - WIDTH/2)
#define STARTY (LCDHEIGHT/2 - HEIGHT/2)


#define _toRadians(deg) (deg * (M_PI / 180.0))

typedef struct {
    rectangle rect, oldRect;
    // signed ints
    int direction, theta, intercept,
        width, height,        
        // fixed is whether it moves on collision
        // hittable is whether it collides
        // collision is a flag, if it is high then there is a 
        // collision and it doesn't check the collision again
        fixed, hittable, collision;
    uint16_t colour;
} movingRectangle;

movingRectangle createRect(startX, startY, width, height, direction, theta, fixed, hittable)
{
    return (movingRectangle){
        .rect = { startX, startX + width, startY, startY + height },
        .oldRect = { 0 },
        .width = width,
        .height = height,
        .direction = direction,
        .theta = theta,
        .intercept = startY - startX*tan(_toRadians(theta)),
        .fixed = fixed,
        .hittable = hittable,
        .collision = -1
    };
}

void printMovingRectangle(movingRectangle *rect)
{
    printf("left: %d, right: %d, top: %d, bottom: %d, theta: %d, dir: %d, int: %d, colid: %d\n",
           rect->rect.left, rect->rect.right, rect->rect.top, rect->rect.bottom, 
           rect->theta, rect->direction, rect->intercept, rect->collision);
}

void moveRect()
{
    static movingRectangle rect = createRect(20, 10, 10, 10, 1, 45, 0, 1);
    
    rect.left += 5;
    rect.right += 5;
    fill_rectangle(rect.rect, BLUE);
}

int main(void)
{
    init_lcd();
    init_debug_uart0();
    movingRectangle rect = createRect(0, 0, 10, 10, 1, 45, 0, 1);
    printMovingRectangle(&rect);
    fill_rectangle(rect.rect, BLUE);

    while (1){
        moveRect();
        _delay_ms(1000);
    }
}

