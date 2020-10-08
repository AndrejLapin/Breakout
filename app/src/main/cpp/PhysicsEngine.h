//
// Created by USER on 2020-10-08.
//

#ifndef BREAKOUTCPP_PHYSICSENGINE_H
#define BREAKOUTCPP_PHYSICSENGINE_H

#include "jni.h"
#include "Paddle.h"


class PhysicsEngine
{
private:

    Paddle *playerPaddle;

public:

    PhysicsEngine(int screenX, int screenY);
    ~PhysicsEngine();

    //void JNICALL Java_com_andrej_breakoutcpp_MainActivity_Init(JNIEnv *, jobject, jint, jint);

protected:


};


#endif //BREAKOUTCPP_PHYSICSENGINE_H
