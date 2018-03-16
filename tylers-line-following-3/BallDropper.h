/*
  BallDropper.h - Library for ball-dropping mechanism, ME210 Final Project 2018.
*/

#ifndef BallDropper_h
#define BallDropper_h

#include "Arduino.h"
#include <Servo.h>

class BallDropper {
    public:
        // constructor
        BallDropper(int pin);

        void dropBalls(int num);

    private:

        void dropBall();
        void rewindServo();

        int _pin;
        int _pos;
        Servo servo;
};

#endif
