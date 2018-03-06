/*
  MotorDriver.h - Library for motor driving control, ME210 Final Project 2018.
  Made to drive a pair of DC motors with an L298
*/

#ifndef MotorDriver_h
#define MotorDriver_h

#include "Arduino.h"

class MotorDriver {
    public:
        // constructor
        MotorDriver(int enA, int in1, int in2, int in3, int in4, int enB);

        // drive control
        void driveForward(int speed);
        void driveBackward(int speed);
        void stop();

        // fine-tuned control
        void adjustLeftSpeed(int delta);
        void adjustRightSpeed(int delta);

        // 90-degree turns
        void turnRight();
        void turnLeft();

    private:
        // retain pin numbers
        int _enA;
        int _in1;
        int _in2;
        int _in3;
        int _in4;
        int _enB;

        // retain base speed for fine-tuned control
        int _baseSpeed;
};

#endif
