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
    
    // if no active collision
    if (rect->collision == -1)
    {
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
    }
    
    setRect(&rect->oldRect, rect->rect.left, rect->rect.right, rect->rect.top, rect->rect.bottom);
    setRect(&rect->rect, left, right, top, bottom);
}


void testBoundaryCollsion(movingRectangle *rect)
{
    // collision with boundaries, only if it doesn't collide with something else
    // x axis collision
    if ((rect->rect.right >= display.width) || (rect->rect.left <= 0)) {
        rect->direction = -rect->direction;
        rect->theta = -rect->theta;
        rect->intercept = rect->rect.top - rect->rect.left*tan(_toRadians(rect->theta));
    }
    
    // y axis collision
    // no direction change
    if ((rect->rect.bottom >= display.height) || (rect->rect.top <= 0)) {
        rect->theta = -rect->theta;
        rect->intercept = rect->rect.top - rect->rect.left*tan(_toRadians(rect->theta));
    }
}

void testCollisions(movingRectangle rects[], int n)
{
    int i, j;
    
    for (i = 0; i < n; i++)
    {
        // if it is not hittable the continue
        // if it is fixed then it will not be affected by colliding so continue
        if (!rects[i].hittable || rects[i].fixed)
            continue;

        for (j = 0; j < n; j++)
        {
            // if i == j then the rectangles are the same
            // see above
            if (i == j || !rects[j].hittable)
                continue;

            // i does not intersect j
            if (!((rects[i].rect.bottom < rects[j].rect.top) ||
                (rects[i].rect.top > rects[j].rect.bottom) ||
                (rects[i].rect.left > rects[j].rect.right) ||
                (rects[i].rect.right < rects[j].rect.left)))
            {
                // collision
                
                if (rects[i].collision != j) {
                    rects[i].theta = -rects[i].theta;
                    rects[i].intercept = rects[i].rect.top - rects[i].rect.left*tan(_toRadians(rects[i].theta));
                }
                
                // set collision flag if not already
                rects[i].collision = j;
            } else {
                // clear collision flag
                if (rects[i].collision == j)
                    rects[i].collision = -1;
            }                
        }
    }
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
    printf("left: %d, right: %d, top: %d, bottom: %d, theta: %d, dir: %d, int: %d, colid: %d\n",
           rect->rect.left, rect->rect.right, rect->rect.top, rect->rect.bottom, 
           rect->theta, rect->direction, rect->intercept, rect->collision);
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