
#include <jni.h>
#include "Engine.h"

Engine *engine;

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Init(JNIEnv *env, jobject,
                                                jint screenX,jint screenY)
{
    engine = new Engine(screenX, screenY);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Update(JNIEnv *env, jobject thiz,
                                                  jlong fps)
{
    engine->Update(fps);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_SetOffsetSet(JNIEnv *env, jobject thiz,
                                                        jboolean value) // sets value of bool offsetSet
{
    engine->SetOffsetSet(value);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_TouchListener(JNIEnv *env, jobject thiz,
                                                         jfloat touchPointX, jfloat touchPointY)
{
    engine->TouchListener(touchPointX, touchPointY);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_ButtonTouchListener(JNIEnv *env, jobject thiz,
                                                               jfloat touchPointX,
                                                               jfloat touchPointY)
{
    engine->ButtonTouchListener(touchPointX, touchPointY);
}

void RestartGame()
{
    engine->Destroy();
    int screenX = engine->GetScreenParameters().x;
    int screenY = engine->GetScreenParameters().y;
    delete engine;
    engine = new Engine(screenX, screenY);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_SetPaddleIsTouched(JNIEnv *env, jobject thiz,
                                                              jboolean value)
{
    engine->SetPaddleIsTouched(value);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_Destroy(JNIEnv *env, jobject thiz)
{
    engine->Destroy();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetLives(JNIEnv *env, jobject thiz)
{
    return engine->GetLives();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetScore(JNIEnv *env, jobject thiz)
{
    return engine->GetScore();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_BrickBreaking(JNIEnv *env, jobject thiz)
{
    return engine->BrickBreaking();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_ShouldAppBeRunning(JNIEnv *env, jobject thiz)
{
    return engine->ShouldAppBeRunning();
}

// =============================== Player Methods =================================

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerTop(JNIEnv *env, jobject thiz)
{
    return engine->GetPlayerTop();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerLeft(JNIEnv *env, jobject thiz)
{
    return engine->GetPlayerLeft();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerRight(JNIEnv *env, jobject thiz)
{
    return engine->GetPlayerRight();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerBottom(JNIEnv *env, jobject thiz)
{
    return engine->GetPlayerBottom();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetPlayerRadius(JNIEnv *env, jobject thiz)
{
    return engine->GetPlayerRadius();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_LaunchBall(JNIEnv *env, jobject thiz,
                                                      jlong holdTime)
{
    engine->LaunchBall(holdTime);
}

// =============================== Ball Methods =================================

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallXPos(JNIEnv *env, jobject thiz)
{
    return engine->GetBallXPos();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallYPos(JNIEnv *env, jobject thiz)
{
    return engine->GetBallYPos();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBallRadius(JNIEnv *env, jobject thiz)
{
    return engine->GetBallRadius();
}

// ================================ Brick Methods ==================================

extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetNumBricks(JNIEnv *env, jobject thiz)
{
    return engine->GetNumberOfBricks();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetIsAlive(JNIEnv *env, jobject thiz, jint index)
{
    return engine->GetIsAlive();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickTop(JNIEnv *env, jobject thiz, jint index)
{
    return engine->GetBrickTop(index);
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickLeft(JNIEnv *env, jobject thiz, jint index)
{
    return engine->GetBrickLeft(index);
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickRight(JNIEnv *env, jobject thiz, jint index)
{
    return engine->GetBrickRight(index);
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetBrickBottom(JNIEnv *env, jobject thiz, jint index)
{
    return engine->GetBrickBottom(index);
}

// Button Methods

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartTop(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonRestartTop();
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartLeft(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonRestartLeft();
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartRight(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonRestartRight();
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartBottom(JNIEnv *env, jobject thiz)
{
    return engine->GetBUttonRestartBottom();
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartText(JNIEnv *env, jobject thiz)
{
    return env->NewStringUTF(engine->GetButtonRestartText().c_str());
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonRestartTextSize(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonRestartTextSize();
}


extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitTop(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonQuitTop();
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitLeft(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonQuitLeft();
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitRight(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonQuitRight();
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitBottom(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonQuitBottom();
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitText(JNIEnv *env, jobject thiz)
{
    return env->NewStringUTF(engine->GetButtonQuitText().c_str());
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_andrej_breakoutcpp_BreakoutEngine_GetButtonQuitTextSize(JNIEnv *env, jobject thiz)
{
    return engine->GetButtonQuitTextSize();
}