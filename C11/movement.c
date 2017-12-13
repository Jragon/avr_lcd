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
    
    printf("left: %d, right: %d, top: %d, bottom: %d\n", left, right, top, bottom);
    
    // hit right boundary
    if (right >= display.width) {
        right = display.width;
        
        rect->direction = -rect->direction;
        rect->theta = -rect->theta;
        rect->intercept = top - left*tan(_toRadians(rect->theta));
        
        printf("RIGHT\n");
    }
    
    // hit bottom boundary
    if (bottom >= display.height) {
        rect->theta = -rect->theta;
        rect->intercept = top - left*tan(_toRadians(rect->theta));
        
        printf("BOTTOM\n");
    }
    
    // hit left boundary
    if (left <= 0){
        rect->direction = -rect->direction;
        rect->theta = -rect->theta;
        rect->intercept = top - left*tan(_toRadians(rect->theta));
        
        printf("LEFT\n");
    }
    
    // hit top boundary
    if (top <= 0){
        rect->theta = -rect->theta;
        rect->intercept = top - left*tan(_toRadians(rect->theta));
        
        printf("TOP\n");
    }
    
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