//
// Created by USER on 2020-10-10.
//

#include <cmath>
#include "Ball.h"


Ball::Ball()
{
    myShape = Circle();

    myMovement = Movement();

    ballLaunched = false;
}

Ball::Ball(int screenX, int screenY, Paddle player)
{
    myShape = Circle(screenX/2, player.GetRect().bottom - screenY / 50, screenY / 50);
    myMovement = Movement();
    ballLaunched = false;
}

Circle Ball::GetCircle()
{
    return myShape;
}

bool Ball::Update(Paddle player)
{
    if(ballLaunched)
    {
        /*myShape.xPos += myMovement.directionX*myMovement.velocity/fps;
        myShape.yPos += myMovement.directionY*myMovement.velocity/fps;*/
        return true;
    }
    else
    {
        myShape.xPos = player.GetRect().left + (player.GetRect().right - player.GetRect().left) /2;
    }
    return false;
}

void Ball::AddVelocity(float velocityToAdd)
{
    myMovement.velocity += velocityToAdd;
}

bool Ball::GetBallLaunched()
{
    return ballLaunched;
}

void Ball::SetBallLaunched(bool value)
{
    ballLaunched = value;
}

void Ball::AddXandYDirections(float addX, float addY)
{
    int sign;
    if (addX != 0)
    {
        myMovement.directionX += addX;
        if(myMovement.directionX > 1 || myMovement.directionX < -1)
        {
            myMovement.directionX = myMovement.directionX > 1 + (myMovement.directionX < -1)*(-1);
        }
        sign = (0 < myMovement.directionY) - (myMovement.directionY < 0);
        myMovement.directionY = (1 - abs(myMovement.directionX))*sign;
    }

    if (addY != 0)
    {
        myMovement.directionY += addY;
        if (myMovement.directionY > 1 || myMovement.directionY < -1)
        {
            myMovement.directionY = myMovement.directionY > 1 + (myMovement.directionY < -1)*(-1);
        }
        sign = (0 < myMovement.directionX) - (myMovement.directionX < 0);
        myMovement.directionX = (1 - abs(myMovement.directionY))*sign;
    }
}

Movement Ball::GetMovement()
{
    return myMovement;
}

void Ball::AddRandomVelocityOnHit()
{
    // random range from -0.05 to 0.05 with increments of 0.01
    AddXandYDirections(((rand() % 11) - 5)/ 100.0f,0);
}

void Ball::AddPosition(float addX, float addY)
{
    myShape.xPos += addX;
    myShape.yPos += addY;
}


