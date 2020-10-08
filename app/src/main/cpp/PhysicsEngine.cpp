//
// Created by USER on 2020-10-08.
//

#include "PhysicsEngine.h"
#include "Paddle.h"
#include "jni.h"

PhysicsEngine::PhysicsEngine(int screenX, int screenY)
{
    playerPaddle = new Paddle(screenX, screenY);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Init(JNIEnv *env, jobject thiz,
        jint screenX,jint screenY)
{
    PhysicsEngine(screenX, screenY);
}

PhysicsEngine::~PhysicsEngine()
{
    delete playerPaddle;
}