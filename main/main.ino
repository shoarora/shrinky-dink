#include "MotorDriver.h"
#include "LSA.h"
#include "BallDropper.h"


// enumerating machine states
enum State {
    APPROACHING_START,
    AT_START,
    APPROACHING_A,
    AT_A,
    APPROACHING_PATENT,
    AT_PATENT,
    APPROACHING_B,
    AT_B,
    APPROACHING_TURN,
    TURNING,
    INVADING,
    DONE
};

// motor one
const int enA = 10;
const int in1 = 9;
const int in2 = 8;
// motor two
const int enB = 5;
const int in3 = 7;
const int in4 = 6;

// motor base speed
const int baseSpeed = 255;

// line sensor pins
const int follow1 = A0;
const int follow2 = A1;
const int north = A2;

// PD constants
// TODO tune these
//const float Kp = 0.0625;
//const float Kd = 0.05;

//const float Kp = 0.015;
//const float Kd = 0.01;

const float Kp = 0.01;
const float Kd = 0.075;

// ball dropper pin
const int dropperPin = A3;

// timer intervals
const int pdInterval = 2000;

// robot controllers
MotorDriver driver = MotorDriver(enA, in1, in2, in3, in4, enB);
LSA lsa = LSA(follow1, follow2, north);
BallDropper dropper = BallDropper(dropperPin);

// interval timers
IntervalTimer pdTimer;

// current state
State state = APPROACHING_START;

void setup() {
//    pdTimer.begin(updateMotors, pdInterval);
    driver.driveForward(baseSpeed);
}

void loop() {
    checkState();
    switch (state) {
        case APPROACHING_START:
            driver.driveForward(baseSpeed);
            break;
        case AT_START:
            state = APPROACHING_A;
            delay(500);
            break;
        case APPROACHING_A:
            // drive forward until arrived at funding round A
            driver.driveForward(baseSpeed);
            break;
        case AT_A:
            // drop balls into funding round A
            driver.stop();
            // TODO move transition to checkState()
            dropper.dropBalls(1);
            state = APPROACHING_PATENT;
            // continue forward after balls dropped
            driver.driveForward(baseSpeed);
            delay(500);
            break;
        case APPROACHING_PATENT:
            // drive forward until arrived at patent office
            driver.driveForward(baseSpeed);
            break;
        case AT_PATENT:
            // do nothing
            Serial.println("at patent");
//            driver.stop();
//            dropper.dropBalls(1);
            state = APPROACHING_B;
//            driver.driveForward(baseSpeed);
            delay(500);
            break;
        case APPROACHING_B:
            // drive forward until arrived at funding round B
            driver.driveForward(baseSpeed);
            break;
        case AT_B:
            // drop balls into funding round B
            driver.stop();
            // TODO move state transtition to checkState()
            dropper.dropBalls(1);
            state = APPROACHING_TURN;
            // continue forward after balls dropped
            driver.driveForward(baseSpeed);
            delay(500);
            break;
        case APPROACHING_TURN:
            // drive forward until we reach the turn
            break;
        case TURNING:
            // turn right toward opponent's garage
            driver.turnRight(baseSpeed);
            // TODO move state transition to checkState()
            state = INVADING;
            break;
        case INVADING:
            // invade
            // TODO move state transition to checkState()
            // TODO figure out when done
            driver.driveForward(baseSpeed);
            delay(9000);
            state = DONE;
            break;
        case DONE:
          driver.stop();
          break;
        default:
            break;
    }
}

void checkState() {
    if (lsa.isSensorReading(2)) {
        switch (state) {
            case APPROACHING_START:
                state = AT_START;
                break;
            case APPROACHING_A:
                state = AT_A;
                break;
            case APPROACHING_PATENT:
                state = AT_PATENT;
                Serial.println("boom");
                break;
            case APPROACHING_B:
                state = AT_B;
                Serial.println("boom boom");
                break;
            case APPROACHING_TURN:
                state = TURNING;
                break;
            default:
                break;
        }
    }
}

void updateMotors() {
    int pd = lsa.calculatePD(Kp, Kd);
    Serial.println(pd);
    if (pd < 0) {
        driver.adjustLeftSpeed(-pd);
    } else {
        driver.adjustRightSpeed(pd);
    }
}
