#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pwm.h"

void * pwmMain(void * arg) {
    printf("hello from thread");
}

typedef struct PwmControl {
    int pin;
    long span;
    long fill;
    int stop;
} PwmControl;

JNIEXPORT jlong JNICALL Java_pipwm4j_Pwm_allocateControlStructure(JNIEnv *env, jobject thisObj, jint pin) {
    long structure = (long) malloc(sizeof(PwmControl));
    ((PwmControl *)structure)->pin = pin;
    return (jlong) structure;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_freeControlStructure(JNIEnv *env, jobject thisObj, jlong controlStructure) {
    free((void *) controlStructure);
    return;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_doPwm(JNIEnv *env, jobject thisObj, jlong controlStructure) {
   printf("Starting thread!\n");
   pthread_t t;
    pthread_start(&t, NULL, pwmMain, NULL);
   return;
}

