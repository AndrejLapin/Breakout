//
// Created by USER on 2020-10-07.
//

#ifndef BREAKOUTCPP_PADDLE_H
#define BREAKOUTCPP_PADDLE_H
#include "Structures.h"
#include "Rect.h"

class Paddle
{
private:
    Rect myShape;

    // paddle length
    float length;
    // paddle height will be responsible for radii of round edges
    // X is far left of the rectangle
    float x;

public:
    Paddle();
    Paddle(int screenX, int screenY);
    //~Paddle();
    Rect GetRect();
    void ChangePaddlePosition(float xOffset);


protected:

};


#endif //BREAKOUTCPP_PADDLE_H
