//
// Created by USER on 2020-10-10.
//

#include "Ball.h"


Ball::Ball()
{
    myShape = Circle();

    // how much force is being applied on axis
    forceX = 1;
    forceY = 0;
    // forceX + forceY must always equal to 1
    velocity = 0;
    ballLaunched = false;
}

Ball::Ball(int screenX, int screenY, Paddle player)
{
    myShape = Circle(screenX/2, player.GetRect().bottom - screenY / 50, screenY / 50);
    forceX = 0;
    forceY = -1;
    velocity = 0;
    ballLaunched = false;
}

Circle Ball::GetCircle()
{
    return myShape;
}

void Ball::Update(long fps, Paddle player)
{
    if(ballLaunched)
    {
        myShape.xPos += forceX*velocity/fps;
        myShape.yPos += forceY*velocity/fps;
    }
    else
    {
        myShape.xPos = player.GetRect().left + (player.GetRect().right - player.GetRect().left) /2;
    }
}

void Ball::AddVelocity(float velocityToAdd)
{
    velocity += velocityToAdd;
}

bool Ball::GetBallLaunched()
{
    return ballLaunched;
}

void Ball::SetBallLaunched(bool value)
{
    ballLaunched = value;
}


