#include "MotorDriver.h"
#include "LSA.h"

/*
 * TODO:
 * add ball drop
 *
 * turn
 *
 * clean up state machine outline
 */

// enumerating machine states
enum State {
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

// motor pins
const int enA = 5;
const int in1 = 6;
const int in2 = 7;
const int in3 = 8;
const int in4 = 9;
const int enB = 10;

// motor base speed
const int baseSpeed = 200;

// line sensor pins
const int follow1 = A0;
const int follow2 = A1;
const int north = A2;

// PD constants
const float Kp = 4.0;
const float Kd = 4.0;

// timer intervals
const int pdInterval = 2000;

// robot controllers
MotorDriver driver = MotorDriver(enA, in1, in2, in3, in4, enB);
LSA lsa = LSA(follow1, follow2, north);

// interval timers
IntervalTimer pdTimer;

// current state
State state = APPROACHING_A;

void setup() {
    pdTimer.begin(updateMotors, pdInterval);
    driver.driveForward(baseSpeed);
}

void loop() {
    checkState();
    switch (state) {
        case APPROACHING_A:
            // drive forward until arrived at funding round A
            driver.driveForward(baseSpeed);
            break;
        case AT_A:
            // drop balls into funding round A
            driver.stop();
            // TODO move transition to checkState()
            // TODO drop balls
            state = APPROACHING_PATENT;
            // continue forward after balls dropped
            driver.driveForward(baseSpeed);
            break;
        case APPROACHING_PATENT:
            // drive forward until arrived at patent office
            break;
        case AT_PATENT:
            // do nothing
            state = APPROACHING_B;
            break;
        case APPROACHING_B:
            // drive forward until arrived at funding round B
            break;
        case AT_B:
            // drop balls into funding round B
            driver.stop();
            // TODO move state transtition to checkState()
            // TODO drop balls
            state = APPROACHING_TURN;
            // continue forward after balls dropped
            driver.driveForward(baseSpeed);
            break;
        case APPROACHING_TURN:
            // drive forward until we reach the turn
            break;
        case TURNING:
            // turn right toward opponent's garage
            driver.turnRight();
            // TODO move state transition to checkState()
            state = INVADING;
            break;
        case INVADING:
            // invade
            // TODO move state transition to checkState()
            // TODO figure out when done
            state = DONE;
            break;
        default:
            break;
    }
}

void checkState() {
    if (lsa.isSensorReading(3)) {
        switch (state) {
            case APPROACHING_A:
                state = AT_A;
                break;
            case APPROACHING_PATENT:
                state = AT_PATENT;
                break;
            case APPROACHING_B:
                state = AT_B;
                break;
            case APPROACHING_TURN:
                state = TURNING;
                break;
            default:
                break;
        }
    }
    if (state == AT_PATENT) {
        state = APPROACHING_B;
    }
}

void updateMotors() {
    int pd = lsa.calculatePD(Kp, Kd);
    driver.adjustLeftSpeed(pd);
    driver.adjustRightSpeed(-pd);
}
