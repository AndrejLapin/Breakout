
#include <jni.h>
#include "Paddle.h"
#include "Ball.h"

Paddle playerPaddle;
Ball ball;
bool offsetSet;
bool paddleIsTouched;
float xOffset;
float xOffsetStart;

// =============================== Engine Methods =================================

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Init(JNIEnv *env, jobject,
                                                jint screenX,jint screenY)
{
    playerPaddle = Paddle(screenX, screenY);
    ball = Ball(screenX, screenY, playerPaddle);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Update(JNIEnv *env, jobject thiz,
        jlong fps)
{
    ball.Update(fps, playerPaddle);
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
    if(holdTime <= 100 && !ball.GetBallLaunched())
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