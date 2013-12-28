//  Based on the example GPIO program by "Dom and Gert"
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "pwm.h"

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map = 0;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

void setup_io();


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
    if (gpio_map == 0) {
	setup_io();
    }
    long structure = (long) malloc(sizeof(PwmControl));
    ((PwmControl *)structure)->pin = pin;
    INP_GPIO(pin);
    OUT_GPIO(pin);
    return (jlong) structure;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_freeControlStructure(JNIEnv *env, jobject thisObj, jlong controlStructure) {
    free((void *) (int) controlStructure);
    return;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_writeControlStructure(JNIEnv *env, jobject thisObj, jlong hControlStructure, jlong span, jlong fill, jboolean stop) {
    PwmControl * structure = (PwmControl *) (int) hControlStructure;
    structure->span = span;
    structure->fill = fill;
    structure->stop = stop;
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_setPinState(JNIEnv *env, jobject thisObj, jint pin, jboolean state) {
    if (state) {
	GPIO_SET = 1 << pin;
    } else {
	GPIO_CLR = 1 << pin;
    }
}

JNIEXPORT void JNICALL Java_pipwm4j_Pwm_doPwm(JNIEnv *env, jobject thisObj, jlong hControlStructure) {
    PwmControl * structure = (PwmControl *) (int) hControlStructure;
    printf("thread started for pin %d\n", structure->pin);
    while (!structure->stop) {
	if (structure->fill > 0) {
    	    GPIO_SET = 1 << structure->pin;
	    usleep(structure->fill);
	}
	GPIO_CLR = 1 << structure->pin;
	usleep(structure->span - structure->fill);
    }
    printf("thread stopped for pin %d\n", structure->pin);
    return;
}

//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);//errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io
