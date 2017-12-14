#pragma once

#include "lcd.h"

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

void testCollisions(movingRectangle rects[], int n);
void moveRectangle(movingRectangle *rect, const int inc);
void setRect(rectangle *rect, int left, int right, int top, int bottom);
void printMovingRectangle(movingRectangle *rect);
void redrawRectangles(movingRectangle rect[], int n);