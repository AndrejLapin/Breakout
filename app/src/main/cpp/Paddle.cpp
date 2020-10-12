
#include "Paddle.h"
#include "jni.h"
#include "Rect.h"


Paddle::Paddle()
{
    length = 0;
    height = 0;

    // puts the paddle right in the middle of the screen
    x = 0;

    // setting top y coordinate
    float y = 0;

    xConstraints = 0;
}

Paddle::Paddle(int screenX, int screenY)
{
    length = screenX/12; // 8.7, 20
    height = screenY/20; // 27, 30

    // puts the paddle right in the middle of the screen
    x = screenX/2 - length/2;

    // setting top y coordinate
    float y = screenY - screenY/19;

    // creating rectangle shape for paddle
    myShape = Rect(x, y, x+length, y+height); // risk of overflow, can allocate memory with new, but have problems with destructors

    xConstraints = screenX;
}

extern "C" JNIEXPORT
Rect Paddle::GetRect()
{
    return myShape;
}

void Paddle::ChangePaddlePosition(float xOffset)
{
    if(x - xOffset > 0 + height/2 && x+length - xOffset < xConstraints - height/2)
    {
        x -= xOffset;
        myShape.left = x;
        myShape.right = x + length;
    }

}

float Paddle::GetRadius()
{
    return (myShape.top-myShape.bottom) /2;
}



/*Paddle::~Paddle()
{
    // call debug method to see if it is deleted right away
    delete myShape;
}*/