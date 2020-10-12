
#include <jni.h>
#include <cmath>
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Button.h"

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
bool brickBreaking;
Brick *bricks[200];
Button *btnRestart;
Button *btnQuit;
Vector2 touchValues;

float where; // debug value

// =============================== Engine Methods =================================

void Init(jint x, jint y);
void SetupLevel(float screenX,float screenY,Paddle playerPaddle);
int FindSign(float value);
void CheckCollisionsAddDistance(long fps, float velocity);
void ProcessMinimalStep(long fps, float velocity);
void AddPositionToBall(long fps, float velocity);
bool CirclesCollide(float x1, float y1, float r1, float x2, float y2, float r2);
float CircleDistanceEntered(float x1, float y1, float r1, float x2, float y2, float r2);
bool CircleRectIntersect(Circle circle, Rect rect);
void Destroy();
void MovePaddle(float touchPointX, float touchPointY);

void GameOver();


void RestartGame();

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Init(JNIEnv *env, jobject,
                                                jint screenX,jint screenY)
{
    Init(screenX, screenY);
}

void Init(int screenX, int screenY)
{
    screenWidth = screenX;
    screenHeight = screenY;
    playerPaddle = new Paddle(screenX, screenY);
    lives = 3;
    score = 0;
    SetupLevel(screenX, screenY, *playerPaddle);
    brickBreaking = true;
    btnRestart = new Button(40, 530, 310, 100, "Restart?", 80);
    btnQuit = new Button(40, 700, 310, 100, "Quit?", 80);

    where = 0; // debug value
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
        for(int row = 3; row < 10; row++)
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

    if(numBricksToDestroy <= 0)
    {
        Destroy();
        playerPaddle = new Paddle(screenWidth, screenHeight);
        btnRestart = new Button(40, 530, 310, 100, "Restart?", 80);
        SetupLevel(screenWidth, screenHeight, *playerPaddle);
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

void CheckCollisionsAddDistance(long fps, float velocity)
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
        else if (CirclesCollide(ballX, ballY,ball->GetCircle().radius,
                                playerPaddle->GetRect().left, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius()))
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
        else if (CirclesCollide(ballX, ballY,ball->GetCircle().radius,
                                playerPaddle->GetRect().right, playerPaddle->GetRect().top-playerPaddle->GetRadius(), playerPaddle->GetRadius()))
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

void GameOver()
{
    brickBreaking = false;
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

bool CirclesCollide(float x1, float y1, float r1, float x2, float y2, float r2) //// TO DO: change parameters so it would only accept 2 circles
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

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_ButtonTouchListener(JNIEnv *env, jobject thiz,
                                                               jfloat touchPointX,
                                                               jfloat touchPointY)
{
    if(!brickBreaking)
    {
        if(CircleRectIntersect(Circle(touchPointX, touchPointY, 0), btnRestart->GetRect()))
        {
            RestartGame();
        }
    }
}

void RestartGame()
{
    Destroy();
    Init(screenWidth, screenHeight);
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
    Destroy();
}

void Destroy()
{
    delete playerPaddle;
    delete ball;
    delete btnRestart;
    for(int i = 0; i < numBricks; i++)
    {
        delete bricks[i];
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetLives(JNIEnv *env, jobject thiz)
{
    return lives;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetScore(JNIEnv *env, jobject thiz)
{
    return score;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_BrickBracking(JNIEnv *env, jobject thiz)
{
    return brickBreaking;
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
        ball->AddVelocity(600);
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

// Button Methods

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartTop(JNIEnv *env, jobject thiz)
{
    return btnRestart->GetRect().top;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartLeft(JNIEnv *env, jobject thiz)
{
    return btnRestart->GetRect().left;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartRight(JNIEnv *env, jobject thiz)
{
    return btnRestart->GetRect().right;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartBottom(JNIEnv *env, jobject thiz)
{
    return btnRestart->GetRect().bottom;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartText(JNIEnv *env, jobject thiz)
{
    return env->NewStringUTF(btnRestart->GetText().c_str());
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartTextSize(JNIEnv *env, jobject thiz)
{
    return btnRestart->GetTextSize();
}


extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitTop(JNIEnv *env, jobject thiz)
{
    return btnQuit->GetRect().top;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitLeft(JNIEnv *env, jobject thiz)
{
    return btnQuit->GetRect().left;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitRight(JNIEnv *env, jobject thiz)
{
    return btnQuit->GetRect().right;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitBottom(JNIEnv *env, jobject thiz)
{
    return btnQuit->GetRect().bottom;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitText(JNIEnv *env, jobject thiz)
{
    return env->NewStringUTF(btnQuit->GetText().c_str());
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitTextSize(JNIEnv *env, jobject thiz)
{
    return btnQuit->GetTextSize();
}