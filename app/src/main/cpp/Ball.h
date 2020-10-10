//
// Created by USER on 2020-10-10.
//

#ifndef BREAKOUTCPP_BALL_H
#define BREAKOUTCPP_BALL_H


#include "Circle.h"
#include "Paddle.h"

class Ball
{
private:
    float forceX, forceY, velocity;
    bool ballLaunched;
    Circle myShape;
public:
    Ball();
    Ball(int screenX, int screenY, Paddle player);
    Circle GetCircle();
    void Update(long fps, Paddle player);
    void AddVelocity(float velocityToAdd);
    bool GetBallLaunched();
    void SetBallLaunched(bool value);

protected:

};


#endif //BREAKOUTCPP_BALL_H
