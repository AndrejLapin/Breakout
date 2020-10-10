
#include <jni.h>
#include <cmath>
#include "Paddle.h"
#include "Ball.h"

Paddle playerPaddle;
Ball ball;
bool offsetSet;
bool paddleIsTouched;
float xOffset;
float xOffsetStart;
int screenWidth;
int screenHeight;

float where; // debug value

// =============================== Engine Methods =================================

int FindSign(float value);
void CalculateFrame(long fps, float velocity);

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Init(JNIEnv *env, jobject,
                                                jint screenX,jint screenY)
{
    screenWidth = screenX;
    screenHeight = screenY;
    playerPaddle = Paddle(screenX, screenY);
    ball = Ball(screenX, screenY, playerPaddle);
    where = 0; // debug value
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Update(JNIEnv *env, jobject thiz,
                                                  jlong fps)
{
    where = ball.GetMovement().directionX;
    if(ball.Update(playerPaddle))
    {
        CalculateFrame(fps, ball.GetMovement().velocity);
    }
    //ball.Update(fps, playerPaddle);
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

void CalculateFrame(long fps, float velocity)
{
    float newBallY = ball.GetCircle().yPos+ball.GetCircle().radius*(FindSign(ball.GetMovement().directionY))+ // ball current y bounds
                     ball.GetMovement().directionY*velocity/fps; // how much it will move on y axis
    float newBallX = ball.GetCircle().xPos+ball.GetCircle().radius*(FindSign(ball.GetMovement().directionX))+ // ball current x bounds
                     ball.GetMovement().directionY*velocity/fps;
    float velocityUsed;
    // checking if the ball hits top of the screen
    if(newBallY < 0 )
    {
        // velocity this frame used before collision
        velocityUsed = (ball.GetMovement().directionY*velocity/fps - newBallY)*fps/ball.GetMovement().directionY;
        // applying all velocity that was used to  come in contact
        ball.AddPosition(ball.GetMovement().directionX*velocityUsed/fps,ball.GetMovement().directionY*velocityUsed/fps);
        // reverses ball Y direction
        ball.AddXandYDirections(0, -ball.GetMovement().directionY*2);
        // adds random velocity on hit to simulate surface roughness
        ball.AddRandomVelocityOnHit();

        CalculateFrame(fps, velocity - velocityUsed);
    }
    // this should be reasserting the ball and deducting a life
    else if(newBallY > screenHeight)
    {
        // velocity used this frame before collision
        velocityUsed = (screenHeight - ball.GetCircle().yPos - ball.GetCircle().radius)*fps/ball.GetMovement().directionY;
        // applying all velocity that was used to  come in contact
        ball.AddPosition(ball.GetMovement().directionX*velocityUsed/fps,ball.GetMovement().directionY*velocityUsed/fps);
        // reverses ball Y direction
        ball.AddXandYDirections(0, -ball.GetMovement().directionY*2);
        // adds random velocity on hit to simulate surface roughness
        ball.AddRandomVelocityOnHit();

        CalculateFrame(fps, velocity - velocityUsed);
    }
    else if(newBallX < 0)
    {
        // velocity this frame used before collision
        velocityUsed = (ball.GetMovement().directionX*velocity/fps - newBallX)*fps/ball.GetMovement().directionX;
        // applying all velocity that was used to  come in contact
        ball.AddPosition(ball.GetMovement().directionX*velocityUsed/fps,ball.GetMovement().directionY*velocityUsed/fps);
        // reverses ball X direction
        ball.AddXandYDirections(-ball.GetMovement().directionX*2, 0);
        // adds random velocity on hit to simulate surface roughness
        ball.AddRandomVelocityOnHit();

        CalculateFrame(fps, velocity - velocityUsed);
    }
    else if (newBallX > screenWidth)
    {
        // velocity used this frame before collision
        velocityUsed = (screenWidth - ball.GetCircle().xPos - ball.GetCircle().radius)*fps/ball.GetMovement().directionX;
        // applying all velocity that was used to  come in contact
        ball.AddPosition(ball.GetMovement().directionX*velocityUsed/fps,ball.GetMovement().directionY*velocityUsed/fps);
        // reverses ball X direction
        ball.AddXandYDirections(-ball.GetMovement().directionX*2, 0);
        // adds random velocity on hit to simulate surface roughness
        ball.AddRandomVelocityOnHit();

        CalculateFrame(fps, velocity - velocityUsed);
    }
    else
    {
        ball.AddPosition(ball.GetMovement().directionX*velocity/fps,ball.GetMovement().directionY*velocity/fps);
    }

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
    if (!paddleIsTouched && touchPointX < playerPaddle.GetRect().right + 90 && touchPointX > playerPaddle.GetRect().left - 90
        && touchPointY < playerPaddle.GetRect().top + 90 && touchPointY > playerPaddle.GetRect().bottom - 90)
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
        playerPaddle.ChangePaddlePosition(xOffset);
        xOffsetStart = touchPointX;
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_SetPaddleIsTouched(JNIEnv *env, jobject thiz,
                                                              jboolean value)
{
    paddleIsTouched = value;
}

// =============================== Player Methods =================================

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerTop(JNIEnv *env, jobject thiz)
{
    return playerPaddle.GetRect().top;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerLeft(JNIEnv *env, jobject thiz)
{
    return playerPaddle.GetRect().left;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerRight(JNIEnv *env, jobject thiz)
{
    return playerPaddle.GetRect().right;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerBottom(JNIEnv *env, jobject thiz)
{
    return playerPaddle.GetRect().bottom;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerRadius(JNIEnv *env, jobject thiz)
{
    return (playerPaddle.GetRect().top - playerPaddle.GetRect().bottom) /2;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_LaunchBall(JNIEnv *env, jobject thiz,
                                                      jlong holdTime)
{
    if(holdTime <= 150 && !ball.GetBallLaunched())
    {
        ball.SetBallLaunched(true);
        ball.AddVelocity(1000);
    }
}

// =============================== Ball Methods =================================

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallXPos(JNIEnv *env, jobject thiz)
{
    return ball.GetCircle().xPos;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallYPos(JNIEnv *env, jobject thiz)
{
    return ball.GetCircle().yPos;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallRadius(JNIEnv *env, jobject thiz)
{
    return ball.GetCircle().radius;
}