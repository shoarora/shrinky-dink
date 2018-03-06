/*
  BallDropper.cpp - Library for ball-dropping mechanism, ME210 Final Project 2018.
*/

#include "Arduino.h"
#include "BallDropper.h"
#include <Servo.h>


BallDropper::BallDropper(int pin) {
    _pin = pin;
    servo.attach(pin);
}

void BallDropper::dropBalls(int num) {
    for (int i=0; i < num; i++) {
        dropBall();
    }
}

void BallDropper::dropBall() {
    if (_pos == 0) {
        for (int pos=0; pos < 90; pos++) {
            servo.write(pos);
            delay(5);  // TODO switch to intervalTimer?
            _pos = pos;
        }
    } else {
        for (int pos=90; pos >= 0; pos--) {
            servo.write(pos);
            delay(5);  // TODO switch to intervalTimer?
            _pos = pos;
        }
    }
}
