/*
  BallDropper.cpp - Library for ball-dropping mechanism, ME210 Final Project 2018.
*/

#include "Arduino.h"
#include "BallDropper.h"
#include <Servo.h>


BallDropper::BallDropper(int pin) {
    _pin = pin;
    pinMode(pin, OUTPUT);
    servo.attach(pin);
    servo.write(180);
}

void BallDropper::dropBalls(int num) {
    for (int i=0; i < num; i++) {
        if (i == 3) {
            rewindServo();
            delay(100);
        }
        dropBall();
    }
    rewindServo();
}

void BallDropper::dropBall() {
    if (_pos == 0) {
      servo.write(_pos+100);
      _pos += 100;
    } else {
      servo.write(_pos+60);
      _pos += 60;
    }
}

void BallDropper::rewindServo() {
    for (int pos=_pos; pos >= 0; pos--) {
        servo.write(pos);
        delay(5);  // TODO switch to intervalTimer?
        _pos = pos;
    }
}
