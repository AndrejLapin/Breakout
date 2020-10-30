//
// Created by USER on 2020-10-20.
//

#include "Engine.h"

Engine::Engine()
{
    screenWidth = 0;
    screenHeight = 0;
    playerPaddle = new Paddle(0, 0);
    lives = 0;
    score = 0;
    SetupLevel(0, 0);
    brickBreaking = true;
    shouldAppRun = true;
    btnRestart = new Button(0, 0, 0, 0, "", 0);
    btnQuit = new Button(0, 0, 0, 0, "", 0);
}

Engine::Engine(int screenX, int screenY)
{
    screenWidth = screenX;
    screenHeight = screenY;
    playerPaddle = new Paddle(screenX, screenY);
    lives = 3;
    score = 0;
    SetupLevel(screenX, screenY);
    shouldAppRun = true;
    btnRestart = new Button(40, 530, 310, 100, "Restart?", 80);
    btnQuit = new Button(40, 660, 195, 100, "Quit?", 80);
}

void Engine::SetupLevel(int screenX, int screenY)
{
    brickBreaking = true;
    ball = new Ball(screenX, screenY, *playerPaddle);
    ballCount = 1;
    numBricks = 0;

    float brickWidth = screenWidth/15;
    float brickHeight = screenHeight/28;

    for (int column = 1; column < 14; column++)
    {
        for(int row = 3; row < 10; row++)
        {
            bricks[numBricks] = new Brick(row, column, brickWidth, brickHeight, 1);
            numBricks ++;
        }
    }
    numBricksToDestroy = numBricks;
}

void Engine::Update(long fps)
{
    if(ball->Update(*playerPaddle))
    {
        ProcessMinimalStep(fps, ball->GetMovement().velocity);
    }

    if(numBricksToDestroy <= 0)
    {
        Destroy();
        playerPaddle = new Paddle(screenWidth, screenHeight);
        btnRestart = new Button(40, 530, 310, 100, "Restart?", 80);
        btnQuit = new Button(40, 660, 310, 100, "Quit?", 80);
        SetupLevel(screenWidth, screenHeight);
    }
}

void Engine::ProcessMinimalStep(long fps, float velocity)
{
    float minimalStep = 1; // decrease minimal step value to get a more precise collision but this in turn increases amount of calculations that has to be done
    velocity -= minimalStep;
    if(velocity < 0)
    {
        velocity += minimalStep;
        CheckCollisionsAddDistance(fps, velocity);
    }
    else
    {
        CheckCollisionsAddDistance(fps, minimalStep);
        ProcessMinimalStep(fps, velocity);
    }
}

void Engine::CheckCollisionsAddDistance(long fps, float velocity)
{
    float ballX = ball->GetCircle().xPos;
    float ballY = ball->GetCircle().yPos;

    // checking for collision with bricks
    for (int i = 0; i < numBricks; ++i)
    {
        if (bricks[i]->GetIsAlive())
        {
            if (CircleRectIntersect(ball->GetCircle(), bricks[i]->GetRect()))
            {
                if(bricks[i]->SubtractLife())
                {
                    score += 10;
                    numBricksToDestroy--;
                }
                //find closest normal
                float yDistance1 = abs(ballY - bricks[i]->GetRect().top);
                float yDistance2 = abs(ballY - bricks[i]->GetRect().bottom);
                float xDistance1 = abs(ballX - bricks[i]->GetRect().right);
                float xDistance2 = abs(ballX - bricks[i]->GetRect().left);
                if(yDistance1 <= yDistance2 && yDistance1 <= xDistance1 && yDistance1 <= xDistance2)
                {
                    ball->AddXandYDirections(0, -ball->GetMovement().directionY*2);
                    AddPositionToBall(fps, velocity);
                    return;

                }
                else if(yDistance2 <= xDistance1 && yDistance2 <= xDistance2)
                {
                    ball->AddXandYDirections(0, -ball->GetMovement().directionY*2);
                    AddPositionToBall(fps, velocity);
                    return;
                }
                else if(xDistance1 <= xDistance2)
                {
                    ball->AddXandYDirections(-ball->GetMovement().directionX*2, 0);
                    AddPositionToBall(fps, velocity);
                    return;
                }
                else
                {
                    ball->AddXandYDirections(-ball->GetMovement().directionX*2, 0);
                    AddPositionToBall(fps, velocity);
                    return;
                }
            }
        }
    }

    // checking if the ball is within playerPaddle bounding region
    if ((ballX+ball->GetCircle().radius < playerPaddle->GetRect().right + playerPaddle->GetRadius() &&
         ballX+ball->GetCircle().radius > playerPaddle->GetRect().left - playerPaddle->GetRadius()) ||
        (ballX-ball->GetCircle().radius < playerPaddle->GetRect().right + playerPaddle->GetRadius() &&
         ballX-ball->GetCircle().radius > playerPaddle->GetRect().left - playerPaddle->GetRadius()))
    {
        float xDistance;
        float yDistance;
        float fullVector;
        //When its within we check for collision
        // Checking rectangle
        if(CircleRectIntersect(ball->GetCircle(), playerPaddle->GetRect()))
        {
            ball->AddXandYDirections(0, -ball->GetMovement().directionY*2);
            ball->AddRandomVelocityOnHit();
            ball->AddVelocity(10);
            AddPositionToBall(fps, velocity);
            return;
        }
            // Checking if the ball hit left round side
        else if (CirclesIntersect(ball->GetCircle(),
                Circle(playerPaddle->GetRect().left, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius())))
        {
            xDistance = ballX - playerPaddle->GetRect().left;
            yDistance = ballY - playerPaddle->GetRect().top-playerPaddle->GetRadius();
            fullVector = abs(xDistance + yDistance);
            ball->AddXandYDirections(xDistance/fullVector, 2*(yDistance/fullVector));

            ball->AddVelocity(10);

            AddPositionToBall(fps, velocity);

            return;
        }
            // Checking if the ball hit right round side, this circle is broken somehow
        else if (CirclesIntersect(ball->GetCircle(),
                                Circle(playerPaddle->GetRect().right, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius())))
        {
            // add to direction vector
            xDistance = ballX - playerPaddle->GetRect().right;
            yDistance = ballY - playerPaddle->GetRect().top-playerPaddle->GetRadius();
            fullVector = abs(xDistance + yDistance);
            ball->AddXandYDirections(xDistance/fullVector, 2*(yDistance/fullVector));

            ball->AddVelocity(10);

            AddPositionToBall(fps, velocity);
            return;
        }

    }
    //collisionWithPaddle = false;
    // checking if the ball hits top of the screen
    if(ballY - ball->GetCircle().radius < 0 )
    {
        // reverses ball Y direction
        ball->AddXandYDirections(0, -ball->GetMovement().directionY*2);
        // adds random velocity on hit to simulate surface roughness
        ball->AddRandomVelocityOnHit();

        AddPositionToBall(fps, velocity);
        return;
    }
    // ball hits bottom of the screen, and you loose a life
    if(ballY + ball->GetCircle().radius > screenHeight)
    {
        ballCount--;
        delete ball;
        if (ballCount <= 0)
        {
            lives--;
            if(lives <= 0)
            {
                GameOver();
            }
            ball = new Ball(screenWidth, screenHeight, *playerPaddle);
            ballCount++;
        }
        return;
    }
    if(ballX - ball->GetCircle().radius < 0)
    {
        // reverses ball X direction
        ball->AddXandYDirections(-ball->GetMovement().directionX*2, 0);
        // adds random velocity on hit to simulate surface roughness
        ball->AddRandomVelocityOnHit();

        AddPositionToBall(fps, velocity);
        return;
    }
    if (ballX + ball->GetCircle().radius > screenWidth)
    {
        // reverses ball X direction
        ball->AddXandYDirections(-ball->GetMovement().directionX*2, 0);
        // adds random velocity on hit to simulate surface roughness
        ball->AddRandomVelocityOnHit();

        AddPositionToBall(fps, velocity);
        return;
    }

    AddPositionToBall(fps, velocity);
}

void Engine::GameOver()
{
    brickBreaking = false;
}

bool Engine::CircleRectIntersect(Circle circle, Rect rect)
{
    // distance between the center of a circle and center of a rectangle
    float circleDistanceX = abs(circle.xPos - (rect.left + rect.GetWidth()/2));
    float circleDistanceY = abs( circle.yPos - (rect.top - rect.GetHeight()/2));

    if(circleDistanceX > (rect.GetWidth()/2 + circle.radius)) { return false; }
    if(circleDistanceY > (rect.GetHeight()/2 + circle.radius)) { return false; }

    if(circleDistanceX <= (rect.GetWidth()/2)) { return true; }
    if(circleDistanceY <= (rect.GetHeight()/2)) { return true; }

    float cornerDistanceSQ = pow((circleDistanceX - rect.GetWidth()/2) + (circleDistanceY - rect.GetHeight()/2),2);

    return (cornerDistanceSQ <= pow((circle.radius),2));
}

bool Engine::CirclesIntersect(Circle circle1, Circle circle2)
{
    float x = circle1.xPos - circle2.xPos;
    float y = circle1.yPos - circle2.yPos;
    float distanceSq = x*x + y*y;
    float radius = circle1.radius + circle2.radius;
    float radiusSq = radius*radius;
    return distanceSq <= radiusSq;
}

void Engine::AddPositionToBall(long fps, float velocity)
{
    ball->AddPosition(ball->GetMovement().directionX*velocity/fps,ball->GetMovement().directionY*velocity/fps);
}

void Engine::SetOffsetSet(bool value)
{
    offsetSet = value;
}

void Engine::TouchListener(float touchPointX, float touchPointY)
{
    if(brickBreaking)
    {
        if (!paddleIsTouched && touchPointX < playerPaddle->GetRect().right + 90 && touchPointX > playerPaddle->GetRect().left - 90
            && touchPointY < playerPaddle->GetRect().top + 90 && touchPointY > playerPaddle->GetRect().bottom - 90)
        {
            paddleIsTouched = true;
        }

        if (paddleIsTouched)
        {
            if(!offsetSet)
            {
                xOffset = 0;
                offsetSet = true;
                xOffsetStart = touchPointX;
            }
            else
            {
                xOffset = xOffsetStart - touchPointX;
            }
            playerPaddle->ChangePaddlePosition(xOffset);
            xOffsetStart = touchPointX;
        }
    }
}

void Engine::ButtonTouchListener(float touchPointX, float touchPointY)
{
    if(!brickBreaking)
    {
        if(CircleRectIntersect(Circle(touchPointX, touchPointY, 0), btnRestart->GetRect()))
        {
            Destroy();
            SetupLevel(screenWidth, screenHeight);
        }
        if(CircleRectIntersect(Circle(touchPointX, touchPointY, 0), btnQuit->GetRect()))
        {
            shouldAppRun = false;
        }
    }
}

// Restart game will probably be native-lib method and will be destroying all objects inside Engine and destroying Engine itself and creating a new one
void Engine::RestartGame()
{
    Destroy();
    // Init function is basically an Engine class constructor
    //Init(screenWidth, screenHeight);
}

void Engine::SetPaddleIsTouched(bool value)
{
    paddleIsTouched = value;
}

void Engine::Destroy()
{
    delete playerPaddle;
    delete ball;
    delete btnRestart;
    delete btnQuit;
    for(int i = 0; i < numBricks; i++)
    {
        delete bricks[i];
    }
}

int Engine::GetLives()
{
    return lives;
}

int Engine::GetScore()
{
    return score;
}

bool Engine::BrickBreaking()
{
    return brickBreaking;
}

bool Engine::ShouldAppBeRunning()
{
    return shouldAppRun;
}

//===================== Player specific methods ===========================

void Engine::MovePaddle(float touchPointX, float touchPointY)
{
        if(!offsetSet)
        {
            xOffset = 0;
            offsetSet = true;
            xOffsetStart = touchPointX;
        }
        else
        {
            xOffset = xOffsetStart - touchPointX;
        }
        playerPaddle->ChangePaddlePosition(xOffset);
        xOffsetStart = touchPointX;
}

float Engine::GetPlayerTop()
{
    return playerPaddle->GetRect().top;
}

float Engine::GetPlayerLeft()
{
    return playerPaddle->GetRect().left;
}

float Engine::GetPlayerRight()
{
    return playerPaddle->GetRect().right;
}

float Engine::GetPlayerBottom()
{
    return playerPaddle->GetRect().bottom;
}

float Engine::GetPlayerRadius()
{
    return playerPaddle->GetRadius();
}

void Engine::LaunchBall(long holdTime)
{
    if(holdTime <= 150 && !ball->GetBallLaunched())
    {
        ball->SetBallLaunched(true);
        ball->AddVelocity(600);
    }
}

//===================== Ball specific methods ====================

float Engine::GetBallXPos()
{
    return ball->GetCircle().xPos;
}

float Engine::GetBallYPos()
{
    return ball->GetCircle().yPos;
}

float Engine::GetBallRadius()
{
    return ball->GetCircle().radius;
}

//========================== Brick specific methods =======================

int Engine::GetNumberOfBricks()
{
    return numBricks;
}

bool Engine::GetIsAlive(int index)
{
    return bricks[index]->GetIsAlive();
}

float Engine::GetBrickTop(int index)
{
    return bricks[index]->GetRect().top;
}

float Engine::GetBrickLeft(int index)
{
    return bricks[index]->GetRect().left;
}

float Engine::GetBrickRight(int index)
{
    return bricks[index]->GetRect().right;
}

float Engine::GetBrickBottom(int index)
{
    return bricks[index]->GetRect().bottom;
}

//=========================== Button methods =====================

float Engine::GetButtonRestartTop()
{
    return btnRestart->GetRect().top;
}

float Engine::GetButtonRestartLeft()
{
    return btnRestart->GetRect().left;
}

float Engine::GetButtonRestartRight()
{
    return btnRestart->GetRect().right;
}

float Engine::GetButtonRestartBottom()
{
    return btnRestart->GetRect().bottom;
}

std::string Engine::GetButtonRestartText()
{
    return btnRestart->GetText();
}

int Engine::GetButtonRestartTextSize()
{
    return btnRestart->GetTextSize();
}


float Engine::GetButtonQuitTop()
{
    return btnQuit->GetRect().top;
}

float Engine::GetButtonQuitLeft()
{
    return btnQuit->GetRect().left;
}

float Engine::GetButtonQuitRight()
{
    return btnQuit->GetRect().right;
}

float Engine::GetButtonQuitBottom()
{
    return btnQuit->GetRect().bottom;
}

std::string Engine::GetButtonQuitText()
{
    return btnQuit->GetText();
}

int Engine::GetButtonQuitTextSize()
{
    return btnQuit->GetTextSize();
}

Vector2 Engine::GetScreenParameters()
{
    return Vector2(screenWidth, screenHeight);
}



