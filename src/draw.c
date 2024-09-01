#include "../dependencies/header.h"

/**
 * drawRect - draw a rectangle
 * @x: x coordinate (top-left corner)
 * @y: y coordinate (top-left corner)
 * @width: rectangle width
 * @height: rectangle height
 * @color: pixel color
*/
void drawRect(int x, int y, int width, int height, color_t color)
{
    int i, j;

    // Loop through each column of the rectangle
    for (i = x; i < x + width; i++) {
        // Loop through each row of the rectangle
        for (j = y; j < y + height; j++) {
            drawPixel(i, j, color);
        }
    }
}
/**
 * drawLine - draw a line between two points
 * @x0: starting x coordinate
 * @y0: starting y coordinate
 * @x1: ending x coordinate
 * @y1: ending y coordinate
 * @color: pixel color
*/
void drawLine(int x0, int y0, int x1, int y1, color_t color)
{
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;
    int longestSideLength = (abs(deltaX) >= abs(deltaY)) ? abs(deltaX) : abs(deltaY);

    float xIncrement = deltaX / (float)longestSideLength;
    float yIncrement = deltaY / (float)longestSideLength;

    float currentX = x0;
    float currentY = y0;

    // Draw the line by iterating over the number of steps required
    for (int i = 0; i <= longestSideLength; i++)
    {
        drawPixel(round(currentX), round(currentY), color);
        currentX += xIncrement;
        currentY += yIncrement;
    }
}
