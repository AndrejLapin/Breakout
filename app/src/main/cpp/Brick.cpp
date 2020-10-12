//
// Created by USER on 2020-10-12.
//

#include "Brick.h"

Brick::Brick()
{
    isAlive = false;
    lives = 0;
    int padding = 0;
    myShape = Rect(0,0,0,0);
}

Brick::Brick(int row, int column, int width, int height, int startingLives)
{
    isAlive = true;
    lives = startingLives;
    int padding = 1;
    myShape = Rect(column*width + padding,
            row*height+ padding,
            column * width + width - padding,
            row * height + height - padding);
}

Rect Brick::GetRect()
{
    return myShape;
}

bool Brick::GetIsAlive()
{
    return isAlive;
}

bool Brick::SubtractLife() // returns true if the brick was just destroyed
{
    lives--;
    if(lives <= 0)
    {
        isAlive = false;
        return true;
    }
    return false;
}
