
#include "Paddle.h"
#include "jni.h"
#include "Rect.h"


Paddle::Paddle()
{
    length = 0;
    float height = 0;

    // puts the paddle right in the middle of the screen
    x = 0;

    // setting top y coordinate
    float y = 0;
}

Paddle::Paddle(int screenX, int screenY)
{
    length = screenX/8.7;
    float height = screenY/27;

    // puts the paddle right in the middle of the screen
    x = screenX/2 - length/2;

    // setting top y coordinate
    float y = screenY - screenY/21.6;

    // creating rectangle shape for paddle
    myShape = Rect(x, y, x+length, y+height); // risk of overflow, can allocate memory with new, but have problems with destructors
}

extern "C" JNIEXPORT
Rect Paddle::GetRect()
{
    return myShape;
}

void Paddle::ChangePaddlePosition(float xOffset)
{
    x -= xOffset;
    myShape.left = x;
    myShape.right = x + length;
}



/*Paddle::~Paddle()
{
    // call debug method to see if it is deleted right away
    delete myShape;
}*/