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
    // todo allocate pin to output
    return (jlong) structure;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_freeControlStructure(JNIEnv *env, jobject thisObj, jlong controlStructure) {
    free((void *) controlStructure);
    return;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_writeControlStructure(JNIEnv *env, jobject thisObj, jlong hControlStructure, jlong span, jlong fill, jboolean stop) {
    PwmControl * structure = (PwmControl *) hControlStructure;
    structure->span = span;
    structure->fill = fill;
    structure->stop = stop;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_setPinState(JNIEnv *env, jobject thisObj, jint pin, jboolean state) {
    // todo set state
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_doPwm(JNIEnv *env, jobject thisObj, jlong hControlStructure) {
    PwmControl * structure = (PwmControl *) hControlStructure;
    printf("thread started for pin %d\n", structure->pin);
    while (!structure->stop) {
	// todo turn on pin
	usleep(structure->fill);
	// todo turn off pin
	usleep(structure->span - strucutre->fill);
    }
    printf("thread stopped for pin %d\n", structure->pin);
    return;
}

