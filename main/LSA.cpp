/*
  LSA.cpp - Library for line sensor array, ME210 Final Project 2018.
*/

#include "Arduino.h"
#include "LSA.h"

const int SENSOR_THRESHOLD = 740;
const int ERROR_BIAS = 0;

LSA::LSA(int follow1, int follow2, int north) {
    // save pin info
    _follow1 = follow1;
    _follow2 = follow2;
    _north = north;
    _lastErr = 0;
    pinMode(follow1, INPUT);
    pinMode(follow2, INPUT);
    pinMode(north, INPUT);
}

int LSA::calculatePD(float Kp, float Kd) {
    // calculate PD with given gain values
    int err = analogRead(_follow1) - analogRead(_follow2) - ERROR_BIAS;
    float delta = Kp * (err + Kd * (err - _lastErr));
    _lastErr = err;
    return (int) delta;
}

bool LSA::isSensorReading(int i) {
    switch (i) {
        case 1:
            return analogRead(_follow1) < SENSOR_THRESHOLD;
        case 2:
            return analogRead(_follow2) < SENSOR_THRESHOLD;
        case 3:
            return analogRead(_north) < SENSOR_THRESHOLD;
        default:
            return false;
    }
}
