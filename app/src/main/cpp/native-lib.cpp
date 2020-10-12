
#include <jni.h>
#include <cmath>
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

Paddle *playerPaddle;
Ball *ball;
bool offsetSet;
bool paddleIsTouched;
float xOffset;
float xOffsetStart;
int screenWidth;
int screenHeight;
int ballCount;
int numBricks;
int numBricksToDestroy; // amount of bricks left to destroy to advance to the next level
int lives;
int score;
Brick *bricks[200];
Vector2 touchValues;

float where; // debug value

// =============================== Engine Methods =================================

void SetupLevel(float screenX,float screenY,Paddle playerPaddle);
int FindSign(float value);
void CheckCollisionsAddDistance(long fps, float velocity);
void ProcessMinimalStep(long fps, float velocity);
void AddPositionToBall(long fps, float velocity);
bool CirclesCollide(float x1, float y1, float r1, float x2, float y2, float r2);
float CircleDistanceEntered(float x1, float y1, float r1, float x2, float y2, float r2);
bool CircleRectIntersect(float x, float y, float r, float left, float top, float right, float bottom);
bool CircleRectIntersect(Circle circle, Rect rect);
void MovePaddle(float touchPointX, float touchPointY);

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Init(JNIEnv *env, jobject,
                                                jint screenX,jint screenY)
{
    screenWidth = screenX;
    screenHeight = screenY;
    playerPaddle = new Paddle(screenX, screenY);
    lives = 3;
    score = 0;
    SetupLevel(screenX, screenY, *playerPaddle);
    where = 0; // debug value
    //collisionWithPaddle = false;
}

void SetupLevel(float screenX,float screenY,Paddle playerPaddle)
{
    ball = new Ball(screenX, screenY, playerPaddle);
    ballCount = 1;
    numBricks = 0;

    float brickWidth = screenWidth/15;
    float brickHeight = screenHeight/28;

    for (int column = 1; column < 14; column++)
    {
        for(int row = 2; row < 10; row++)
        {
            bricks[numBricks] = new Brick(row, column, brickWidth, brickHeight, 1);
            numBricks ++;
        }
    }
    numBricksToDestroy = numBricks;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Update(JNIEnv *env, jobject thiz,
                                                  jlong fps)
{
    if(ball->Update(*playerPaddle))
    {
        ProcessMinimalStep(fps, ball->GetMovement().velocity);
    }
}

int FindSign(float value)
{
    return (0 < value)-(0 > value);
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_DebugValue(JNIEnv *env, jobject thiz, jfloat value)
{
    // ((rand() % 11) - 5)/ 100.0f
    return where;
}

// to have linear interpolation of my ball
void ProcessMinimalStep(long fps, float velocity)
{
    float minimalStep = 0.5; // decrease minimal step value to get a more precise collision
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

void CheckCollisionsAddDistance(long fps, float velocity)
{

    float ballX = ball->GetCircle().xPos;
    float ballY = ball->GetCircle().yPos;

    // checking for collision with bricks
    for (int i = 0; i < numBricks; ++i)
    {
        if (bricks[i]->GetIsAlive())
        {
            if (CircleRectIntersect(ball->GetCircle(), bricks[i]->GetRect())) //parameters for old function ballX, ballY, ball->GetCircle().radius, bricks[i]->GetRect().left, bricks[i]->GetRect().top, bricks[i]->GetRect().right, bricks[i]->GetRect().bottom
            {
                if(bricks[i]->SubtractLife())
                {
                    numBricksToDestroy--;
                }
                //firs we need to calculate collision normal
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
        if(CircleRectIntersect(ball->GetCircle(), playerPaddle->GetRect())) // old parameters for old function ballX, ballY, ball->GetCircle().radius,playerPaddle->GetRect().left, playerPaddle->GetRect().top, playerPaddle->GetRect().right, playerPaddle->GetRect().bottom
        {
            //collisionWithPaddle = true;
            // velocity used this call before collision
            //velocityUsed = (playerPaddle->GetRect().bottom - ball->GetCircle().yPos - ball->GetCircle().radius)*fps/ball->GetMovement().directionY;

            ball->AddXandYDirections(0, -ball->GetMovement().directionY*2);
            ball->AddRandomVelocityOnHit();
            ball->AddVelocity(10);
            AddPositionToBall(fps, velocity);
            return;
        }
        // Checking if the ball hit left round side
        else if (CirclesCollide(ballX, ballY,ball->GetCircle().radius,
                                playerPaddle->GetRect().left, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius()))
        {
            //collisionWithPaddle = true;
            // velocity used this call before collision
            //velocityUsed = velocity - CircleDistanceEntered(newBallXPos, newBallYPos,ball->GetCircle().radius, playerPaddle->GetRect().left, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius());

            xDistance = ballX - playerPaddle->GetRect().left;
            yDistance = ballY - playerPaddle->GetRect().top-playerPaddle->GetRadius();
            fullVector = abs(xDistance + yDistance);
            ball->AddXandYDirections(xDistance/fullVector, 2*(yDistance/fullVector));

            ball->AddVelocity(10);

            AddPositionToBall(fps, velocity);

            return;
        }
        // Checking if the ball hit right round side, this circle is broken somehow
        else if (CirclesCollide(ballX, ballY,ball->GetCircle().radius,
                                playerPaddle->GetRect().right, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius()))
        {
            //collisionWithPaddle = true;
            // velocity used this call before collision
            //velocityUsed = velocity - CircleDistanceEntered(newBallXPos, newBallYPos,ball->GetCircle().radius,playerPaddle->GetRect().right, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius());

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
        // velocity this call used before collision
        //velocityUsed = (ball->GetMovement().directionY*velocity/fps - newBallY)*fps/ball->GetMovement().directionY;

        // reverses ball Y direction
        ball->AddXandYDirections(0, -ball->GetMovement().directionY*2);
        // adds random velocity on hit to simulate surface roughness
        ball->AddRandomVelocityOnHit();

        AddPositionToBall(fps, velocity);
        return;
    }
    // this should be reasserting the ball and deducting a life
    if(ballY + ball->GetCircle().radius > screenHeight)
    {
        ballCount--;
        delete ball;
        if (ballCount <= 0)
        {
            ball = new Ball(screenWidth, screenHeight, *playerPaddle);
            ballCount++;
        }
        return;
    }
    if(ballX - ball->GetCircle().radius < 0)
    {
        // velocity this call used before collision
        //velocityUsed = (ball->GetMovement().directionX*velocity/fps - newBallX)*fps/ball->GetMovement().directionX;

        // reverses ball X direction
        ball->AddXandYDirections(-ball->GetMovement().directionX*2, 0);
        // adds random velocity on hit to simulate surface roughness
        ball->AddRandomVelocityOnHit();

        AddPositionToBall(fps, velocity);
        return;
    }
    if (ballX + ball->GetCircle().radius > screenWidth)
    {
        // velocity used this call before collision
        //velocityUsed = (screenWidth - ball->GetCircle().xPos - ball->GetCircle().radius)*fps/ball->GetMovement().directionX;

        // reverses ball X direction
        ball->AddXandYDirections(-ball->GetMovement().directionX*2, 0);
        // adds random velocity on hit to simulate surface roughness
        ball->AddRandomVelocityOnHit();

        AddPositionToBall(fps, velocity);
        return;
    }

    AddPositionToBall(fps, velocity);

}

bool CircleRectIntersect(Circle circle, Rect rect)
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

bool CircleRectIntersect(float x, float y, float r, float left, float top, float right, float bottom)
{
    float distanceSQToBottomLeft = pow((x - left),2) + pow((y - bottom),2);
    float distanceSQToBottomRight = pow((x - right),2) + pow((y - bottom),2);
    float distanceSQToTopLeft = pow((x - left),2) + pow((y - top),2);
    float distanceSQToTopRight = pow((x - right),2) + pow((y - top),2);
    float radiusSq = r*r;
    return (x < right && x > left  && y < top && y > bottom) ||
            (distanceSQToBottomLeft <= radiusSq) || (distanceSQToBottomRight <= radiusSq) ||
            (distanceSQToTopLeft <= radiusSq) || (distanceSQToTopRight <= radiusSq);
}

bool CirclesCollide(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float x = x1 - x2;
    float y = y1 - y2;
    float distanceSq = x*x + y*y;
    float radius = r1 + r2;
    float radiusSq = radius*radius;
    return distanceSq <= radiusSq;
}

float CircleDistanceEntered(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float x = x1 - x2;
    float y = y1 - y2;
    float distanceSq = x*x + y*y;
    float radius = r1 + r2;
    float radiusSq = radius*radius;
    return sqrt(radiusSq-distanceSq);
}

void AddPositionToBall(long fps, float velocity)
{
    ball->AddPosition(ball->GetMovement().directionX*velocity/fps,ball->GetMovement().directionY*velocity/fps);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_SetOffsetSet(JNIEnv *env, jobject thiz,
                                                        jboolean value) // sets value of bool offsetSet
{
    offsetSet = value;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_TouchListener(JNIEnv *env, jobject thiz,
                                                         jfloat touchPointX, jfloat touchPointY)
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
    // used this before to try to prevent the paddle from moving into the ball
    //touchValues.x = touchPointX;
    //touchValues.y = touchPointY;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_SetPaddleIsTouched(JNIEnv *env, jobject thiz,
                                                              jboolean value)
{
    paddleIsTouched = value;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Destroy(JNIEnv *env, jobject thiz)
{
    delete playerPaddle;
    delete ball;
    for(int i = 0; i < numBricks; i++)
    {
        delete bricks[i];
    }
}

// =============================== Player Methods =================================

void MovePaddle(float touchPointX, float touchPointY)
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

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerTop(JNIEnv *env, jobject thiz)
{
    return playerPaddle->GetRect().top;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerLeft(JNIEnv *env, jobject thiz)
{
    return playerPaddle->GetRect().left;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerRight(JNIEnv *env, jobject thiz)
{
    return playerPaddle->GetRect().right;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerBottom(JNIEnv *env, jobject thiz)
{
    return playerPaddle->GetRect().bottom;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerRadius(JNIEnv *env, jobject thiz)
{
    return playerPaddle->GetRadius();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_LaunchBall(JNIEnv *env, jobject thiz,
                                                      jlong holdTime)
{
    if(holdTime <= 150 && !ball->GetBallLaunched())
    {
        ball->SetBallLaunched(true);
        ball->AddVelocity(500);
    }
}

// =============================== Ball Methods =================================

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallXPos(JNIEnv *env, jobject thiz)
{
    return ball->GetCircle().xPos;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallYPos(JNIEnv *env, jobject thiz)
{
    return ball->GetCircle().yPos;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallRadius(JNIEnv *env, jobject thiz)
{
    return ball->GetCircle().radius;
}

// ================================ Brick Methods ==================================

extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetNumBricks(JNIEnv *env, jobject thiz)
{
    return numBricks;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetIsAlive(JNIEnv *env, jobject thiz, jint index)
{
    return bricks[index]->GetIsAlive();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickTop(JNIEnv *env, jobject thiz, jint index)
{
    return bricks[index]->GetRect().top;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickLeft(JNIEnv *env, jobject thiz, jint index)
{
    return bricks[index]->GetRect().left;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickRight(JNIEnv *env, jobject thiz, jint index)
{
    return bricks[index]->GetRect().right;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickBottom(JNIEnv *env, jobject thiz, jint index)
{
    return bricks[index]->GetRect().bottom;
}