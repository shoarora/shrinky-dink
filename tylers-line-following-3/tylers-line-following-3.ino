#include "BallDropper.h"
#include <Servo.h>

IntervalTimer stepTimer;
IntervalTimer rightTimer;
const int TIMESTEP = 2e4;
const int RIGHTTURNTIME = 5e6;

// tape sensors
const int leftTape = A0;
const int middleTape = A1;
const int rightTape = A2;
int BLACKT = 300;
int WHITET = 800;
char leftCol;
char midCol;
char rightCol;

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
const int motorABias = 0;
const int motorBBias = 30;

const int dropperPin = A3;
BallDropper dropper = BallDropper(dropperPin);
char colors;

bool shouldITurn = false;

char previousTurn; // stores state of most recent error: -1 = left, +1 = right
char shouldIGo; // 0 for straight, 1 for straight-left, 2 for straight-right,
                    // 4 for left, 5 for right, 3 for ignore
int forwardsInARow;

// functions
void drive();
char readSensors();
void driveForward();
void turnLeft();
void turnRight();
void stopRobot();
void fullRightTurn();

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(dropperPin, OUTPUT);
  previousTurn = 0;
  shouldIGo = 0;
  forwardsInARow = 0;
  // // Serial.println("Program begun");
  // put your setup code here, to run once:
  stepTimer.begin(drive, TIMESTEP);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void drive() {
  if(forwardsInARow > 4) {
    previousTurn = 0;
  }
  if(shouldIGo == 3) {
    colors = readSensors();
    switch(colors) {
      case 0: // (   ) all white
      case 3: // ( - ) a line to ignore
        if(previousTurn == -1) {
          turnLeft();
          shouldIGo = 0;
        } else if(previousTurn == 1) {
          turnRight();
          shouldIGo = 0;
        } else {
          driveForward();
        }
        break;
      case 2: // (  B)
      case 1: // (  -)
      case 5: // ( -B)
      case 11: //(- B)
      case 7: // ( B-)
      case 8: // ( BB)
      case 25: //(BB-)
      case 14: //(--B)
        // Serial.println("R");
        // turn right
        turnRight();
        shouldIGo = 0;
        previousTurn = 1;
        break;
      case 6: // ( B )
      case 16: //(-B-)
        // Serial.println("F");
        // go straight: this is good!
        driveForward();
        break;
      case 18: //(B  )
      case 9: // (-  )
      case 21: //(B- )
      case 19: //(B -)
      case 15: //(-B )
      case 24: //(BB )
      case 17: //(-BB)
      case 22: //(B--)
        // Serial.println("L");
        // turn left
        turnLeft();
        shouldIGo = 0;
        previousTurn = -1;
        break;
      case 20: //(B B)
        // Serial.print("a");
      case 23: //(B-B)
        // Serial.print("b");
      case 26: //(BBB)
        // Serial.println("B");
        driveForward();
        // either you're way off
        // go straight for now
        // or you're at the turning point
        // TODO: code turning point
        // these shouldn't be cascaded but they are similar for now
        break;
      case 13: //(---)
      case 12: //(-- )
      case 4: // ( --)
      case 10: //(- -)
        // Serial.println("D");
        // at drop point!
        // drop balls?
        stepTimer.end();
        stopRobot();
        dropper.dropBalls(1);
        driveForward();
        delay(1000);
        stepTimer.begin(drive, TIMESTEP); // issue is right here
        break;
      case 30: //(- -)
        // turn right 90 degrees
        // go forward
        // die
        if(shouldITurn) {
          stepTimer.end();
          fullRightTurn();
          driveForward();
          delay(9000);
          stopRobot();
        } else {
          shouldITurn = true;
        }
    }
  } else if(shouldIGo < 3) {
    // Serial.println("1");
    driveForward();
    shouldIGo += 3;
  } else if(shouldIGo == 4) {
    // Serial.println("4");
    turnLeft();
    shouldIGo = 3;
  } else if(shouldIGo == 5) {
    // Serial.println("5");
    turnRight();
    shouldIGo = 3;
  }
}

char readSensors() {
  char colors;
  int leftRead = analogRead(leftTape);
  int midRead = analogRead(middleTape);
  int rightRead = analogRead(rightTape) - 100;
  int meanRead = (leftRead + rightRead + midRead)/3;
  long varead = abs(leftRead - meanRead) + abs(rightRead - meanRead) + abs(rightRead - meanRead);
  if(varead < 400) {
    // either it's at the gray tape
    if(meanRead > 800) {
      colors = 0;
    } else if(meanRead < 300 && rightRead - leftRead > 500) {
      colors = 30;
    } else {
      colors = 4;
    }
    // or at the turning point
    // or lost the line
  } else {
    if(leftRead + rightRead - 2*midRead > 700) {
      colors = 6;
    } else if(rightRead - leftRead > 500) {
      colors = 18; // turn left
    } else if(leftRead - rightRead > 500) {
      if(midRead - rightRead > 0) {
        colors = 2; // turn right
      } else {
        colors = 30;
      }
    }
  }
  return colors;
}

void driveForward() {
    // set motors to go forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // power motors
    analogWrite(enA, baseSpeed - motorABias);
    analogWrite(enB, baseSpeed - motorBBias);
    forwardsInARow++;
}

void turnLeft() {
    // set motors to go forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // power motors
    analogWrite(enA, 0);
    analogWrite(enB, baseSpeed - motorBBias);
    forwardsInARow = 0;
}

void turnRight() {
    // set motors to go forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // power motors
    analogWrite(enA, baseSpeed - motorABias);
    analogWrite(enB, 0);
    forwardsInARow = 0;
}

void stopRobot() {
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

void fullRightTurn() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // power motors
    analogWrite(enA, baseSpeed - motorABias);
    analogWrite(enB, baseSpeed - motorBBias);

    delay(1400);
    stopRobot();
}

