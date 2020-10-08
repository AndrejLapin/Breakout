
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

    // setting paddle speed
    paddleSpeed = 0;

    currentMove = Stop;
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
    myShape = new Rect(x, y, x+length, y+height);

    // setting paddle speed
    paddleSpeed = screenX/1.92;

    currentMove = Stop;
}

extern "C" JNIEXPORT
Rect Paddle::GetRect()
{
    return *myShape;
}

Paddle::~Paddle()
{
    delete myShape;
}