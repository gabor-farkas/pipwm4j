pipwm4j
=======

A JNI library with a Java wrapper that enables fast PWM on any digital GPIO output pin

The native part relies on the low-level memory mapped solution found here: http://elinux.org/RPi_Low-level_peripherals, by Gert van Loo & Dom.
I'm not familiar with C and C++ so my code might be quite ugly. And it is unstable, so you have to expect JVM crashes in some cases You've been warned! :)

### Usage
Using it through the provided Pwm class is simple:

#### Simple digital output
```
Pwm pin18 = new Pin(18);
pwm.setState(true);
[...]
pwm.close();
```

The Pwm class implements the Java 7 Closeable interface.

#### Doing pulse with modulation
```
Pwm pwm18 = new Pin(18);
pwm.start();    // start the thread
pwm.setParams(1000, 100);    // 100us out of 1000us will be filled
[...]
pwm.close()
```

### How it works
I tried starting a pwm thread from native code but I didn't succeed, so I decided to start the tread in Java. First, you have to allocate a control structure for the specific pin.
The control structure is returned as long, and it is actually a pointer. The structure contains the pin number, the number of microsecs one period lasts, and the number of microsecs it should be in the HIGH state.
The native code uses 'usleep' for timing, so it's not exact. The control structure also contains a flag if the while loop should be stopped. If it stops, the native method returns and the Java thread terminates.

### An example
I created a christmas tree led light decoration: https://www.youtube.com/watch?v=27wDgazy8ho . I first tried to use pi4j to do pwm, but it turned out to be too slow for some reason.