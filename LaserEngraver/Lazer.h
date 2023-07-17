#include "Configure.h"

class Lazer {
private:
  const bool XForward = false;
  const bool YForward = true;
  const int dirPinX;
  const int stepperPinX;
  const int dirPinY;
  const int stepperPinY;
  int speed = conf::speed;
  int currentX;
  int currentY;

  void steppersOff() {
    digitalWrite(stepperPinX, LOW);
    digitalWrite(stepperPinY, LOW);
  }

  bool checkTimer(unsigned long timer, int delay) {
    return (millis() - timer) >= delay;
  }

  void stepBy(int dirPin, bool dir, int stepperPin, int steps) {
    digitalWrite(dirPin, dir);
    for (int i = 0; i < steps; i++) {
      digitalWrite(stepperPin, HIGH);
      delayMicroseconds(speed);
      digitalWrite(stepperPin, LOW);
      delayMicroseconds(speed);
    }
  }

  void stepX(bool dir, int steps) {
    stepBy(dirPinX, dir, stepperPinX, steps);
  }

  void stepY(bool dir, int steps) {
    stepBy(dirPinY, dir, stepperPinY, steps);
  }

public:
  Lazer(int dirPinX, int stepperPinX, int dirPinY, int stepperPinY)
    : dirPinX(dirPinX), stepperPinX(stepperPinX), dirPinY(dirPinY), stepperPinY(stepperPinY) {
    this->currentX = 0;
    this->currentY = 0;
  }

  void lineTo(int x, int y) {
    int difX = x - this->currentX;
    int difY = y - this->currentY;
    this->currentX += difX;
    this->currentY += difY;
    bool xDir = (difX >= 0) ? XForward : !XForward;
    bool yDir = (difY >= 0) ? YForward : !YForward;
    difX = abs(difX);
    difY = abs(difY);

    if (difX == 0 || difY == 0) {
      stepX(xDir, difX);
      stepY(yDir, difY);
      return;
    }

    int xDelay = 16;
    int yDelay = 16;

    if (difX > difY) {
      xDelay = speed / 1000;
      float multiplier = (float)difX / difY;
      yDelay = xDelay * multiplier;
    } else {
      yDelay = speed / 1000;
      float multiplier = (float)difY / difX;
      xDelay = yDelay * multiplier;
    }

    unsigned long xTimer = millis();
    unsigned long yTimer = millis();

    digitalWrite(dirPinX, xDir);
    digitalWrite(dirPinY, yDir);

    while (difX > 0 || difY > 0) {
      steppersOff();
      if (difX > 0 && checkTimer(xTimer, xDelay)) {
        digitalWrite(stepperPinX, HIGH);
        difX--;
        xTimer = millis();
      }
      if (difY > 0 && checkTimer(yTimer, yDelay)) {
        digitalWrite(stepperPinY, HIGH);
        difY--;
        yTimer = millis();
      }
    }
    steppersOff();
  }

  void lineToH(int x) {
    int difX = x - this->currentX;
    this->currentX += difX;
    bool xDir = (difX >= 0) ? XForward : !XForward;
    difX = abs(difX);
    stepX(xDir, difX);
  }

  void lineToV(int y) {
    int difY = y - this->currentY;
    this->currentY += difY;
    bool yDir = (difY >= 0) ? YForward : !YForward;
    difY = abs(difY);
    stepY(yDir, difY);
  }

  void reset() {
    lineTo(0, 0);
  }

  int getX() {
    return currentX;
  }

  int getY() {
    return currentY;
  }
};
