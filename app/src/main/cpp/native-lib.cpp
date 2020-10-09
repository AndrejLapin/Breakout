#include <jni.h>
#include "Paddle.h"
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_andrej_breakoutcpp_MainActivity_stringFromJNI(JNIEnv* env,jobject /* this */)
{
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}