/*
 * movement.c
 *
 * Created: 13/12/2017 13:55:24
 *  Author: RoryWork
 */

#include <math.h>
#include <stdio.h>

#include <avr/io.h>

#include "lcd.h"
#include "movement.h"


void moveRectangle(movingRectangle *rect, const int inc)
{
    int right, left, top, bottom;

    right = rect->rect.right + rect->direction*inc;
    left = rect->rect.left + rect->direction*inc;
    
    top = tan(_toRadians(rect->theta))*left + rect->intercept;
    bottom = top + rect->width;
    
    // x axis collision
    if ((right >= display.width) || (left <= 0)) {
        rect->direction = -rect->direction;
        rect->theta = -rect->theta;
        rect->intercept = top - left*tan(_toRadians(rect->theta));
    }
    
    // y axis collision
    // no direction change
    if ((bottom >= display.height) || (top <= 0)) {
        rect->theta = -rect->theta;
        rect->intercept = top - left*tan(_toRadians(rect->theta));
    }
    
    setRect(&rect->oldRect, rect->rect.left, rect->rect.right, rect->rect.top, rect->rect.bottom);
    setRect(&rect->rect, left, right, top, bottom);
}


void setRect(rectangle *rect, int left, int right, int top, int bottom)
{
    rect->left = left;
    rect->right = right;
    rect->top = top;
    rect->bottom = bottom;
}

void printMovingRectangle(movingRectangle *rect)
{
    printf("left: %d, right: %d, top: %d, bottom: %d, theta: %d, dir: %d, int: %d\n",
           rect->rect.left, rect->rect.right, rect->rect.top, rect->rect.bottom, 
           rect->theta, rect->direction, rect->intercept);
}

void redrawRectangles(movingRectangle rect[], int n)
{
    int i;
    
    for (i = 0; i < n; i++)
    {
        fill_rectangle(rect[i].oldRect, display.background);
        fill_rectangle(rect[i].rect, rect[i].colour);
    }
}