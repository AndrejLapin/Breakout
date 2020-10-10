//
// Created by USER on 2020-10-10.
//

#ifndef BREAKOUTCPP_BALL_H
#define BREAKOUTCPP_BALL_H


#include "Circle.h"
#include "Paddle.h"
#include "Movement.h"

class Ball
{
private:
    Movement myMovement;
    bool ballLaunched;
    Circle myShape;
public:
    Ball();
    Ball(int screenX, int screenY, Paddle player);
    Circle GetCircle();
    bool Update(Paddle player);
    void AddVelocity(float velocityToAdd);
    bool GetBallLaunched();
    void SetBallLaunched(bool value);
    void AddXandYDirections(float addX, float addY);
    Movement GetMovement();
    void AddRandomVelocityOnHit();
    void AddPosition(float addX, float addY);

protected:

};


#endif //BREAKOUTCPP_BALL_H
