#include <jni.h>

JNIEXPORT jlong JNICALL Java_pipwm4j_Pwm_allocateControlStructure(JNIEnv *, jobject, jint pin);
JNIEXPORT void JNICALL Java_pipwm4j_Pwm_writeControlRegister(JNIEnv *, jobject, jlong pwmSpan, jlong fill, jboolean stop);
JNIEXPORT void JNICALL Java_pipwm4j_Pwm_doPwm(JNIEnv *, jobject, jlong controlRegister);
JNIEXPORT void JNICALL Java_pipwm4j_Pwm_freeControlStructure(JNIEnv *, jobject, jlong controlRegister);