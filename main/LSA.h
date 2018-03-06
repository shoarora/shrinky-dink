/*
  LSA.h - Library for line sensor array, ME210 Final Project 2018.
  https://www.robotshop.com/letsmakerobots/opb704-line-sensor circuit reference
*/

#ifndef LSA_h
#define LSA_h

#include "Arduino.h"

class LSA {
    public:
        // constructor
        LSA(int follow1, int follow2, int north);

        // calculate PD delta
        int calculatePD(float Kp, float Kd);

        // get sensor state
        bool isSensorReading(int i);

    private:
        // retain pin numbers
        int _follow1;
        int _follow2;
        int _north;

        // retain last error
        int _lastErr;
};

#endif
