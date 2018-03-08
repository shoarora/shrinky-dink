/*
  MotorDriver.cpp - Library for motor driving control, ME210 Final Project 2018.
  Made to drive a pair of DC motors with an L298
*/

#include "Arduino.h"
#include "MotorDriver.h"

const int motorABias = 0;
const int motorBBias = 30;

MotorDriver::MotorDriver(int enA, int in1, int in2, int in3, int in4, int enB) {
    // save pin info
    _enA = enA;
    _in1 = in1;
    _in2 = in2;
    _in3 = in3;
    _in4 = in4;
    _enB = enB;
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enB, OUTPUT);
}

void MotorDriver::driveForward(int speed) {
    // set motors to go forward
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);

    // save base speed
    _baseSpeed = speed;

    // power motors
    analogWrite(_enA, speed - motorABias);
    analogWrite(_enB, speed - motorBBias);
}

void MotorDriver::driveBackward(int speed) {
    // set motors to go backward
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);

    // save base speed
    _baseSpeed = speed;

    // power motors
    analogWrite(_enA, speed - motorABias);
    analogWrite(_enB, speed - motorBBias);
}

void MotorDriver::stop() {
    analogWrite(_enA, 0);
    analogWrite(_enB, 0);
}

void MotorDriver::adjustLeftSpeed(int delta) {
    int newSpeed = _baseSpeed - delta;

    // anti-windup
    if (newSpeed > 255) {
        newSpeed = 255;
    }
    if (newSpeed < 0) {
        newSpeed = 0;
    }

    analogWrite(_enA, newSpeed - motorABias);
}

void MotorDriver::adjustRightSpeed(int delta) {
    int newSpeed = _baseSpeed - delta;

    // anti-windup
    if (newSpeed > 255) {
        newSpeed = 255;
    }
    if (newSpeed < 180) {
        newSpeed = 0;
    }

    analogWrite(_enB, newSpeed - motorBBias);
}

void MotorDriver::turnLeft() {
    // TODO
}

void MotorDriver::turnRight(int speed) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);

    // save base speed
    _baseSpeed = speed;

    // power motors
    analogWrite(_enA, speed - motorABias);
    analogWrite(_enB, speed - motorBBias);

    delay(1600);
    stop();
}
