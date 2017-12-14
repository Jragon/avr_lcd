#pragma once

#include "lcd.h"

#define _toRadians(deg) (deg * (M_PI / 180.0))

typedef struct {
    rectangle rect, oldRect;
    // signed ints
    int direction, theta, intercept, width, height;
} movingRectangle;

void moveRectangle(movingRectangle *rect, const int inc);
void setRect(rectangle *rect, int left, int right, int top, int bottom);
void printMovingRectangle(movingRectangle *rect);
void drawRectangles(rectangle *rect, int n);