class Lazer {
  private:
    const boolean XForward = false;
    const boolean YForward = true;
    const int dirPinX;
    const int stepperPinX;
    const int dirPinY;
    const int stepperPinY;
    int speed = 30000;
    int currentX;
    int currentY;

    void steppersOff() {
      digitalWrite(stepperPinX, LOW);
      digitalWrite(stepperPinY, LOW);
    }

    bool checkTimer(unsigned long timer, int delay) {
      return (millis() - timer) >= delay;
    }

  public:
    Lazer(int dirPinX, int stepperPinX, int dirPinY, int stepperPinY)
      : dirPinX(dirPinX), stepperPinX(stepperPinX), dirPinY(dirPinY), stepperPinY(stepperPinY) {
      this->currentX = 0;
      this->currentY = 0;
    }

    void stepX(boolean dir, int steps) {
      digitalWrite(dirPinX, dir);
      for (int i = 0; i < steps; i++) {
        digitalWrite(stepperPinX, HIGH);
        delayMicroseconds(speed);
        digitalWrite(stepperPinX, LOW);
        delayMicroseconds(speed);
      }
    }

    void stepY(boolean dir, int steps) {
      digitalWrite(dirPinY, dir);
      for (int i = 0; i < steps; i++) {
        digitalWrite(stepperPinY, HIGH);
        delayMicroseconds(speed);
        digitalWrite(stepperPinY, LOW);
        delayMicroseconds(speed);
      }
    }

    void lineTo(int x, int y) {
      int difX =  x - this->currentX;
      int difY =  y - this->currentY;
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


    //    void lineTo(int x, int y) {
    //      int difX =  x - this->currentX;
    //      int difY =  y - this->currentY;
    //      this->currentX += difX;
    //      this->currentY += difY;
    //      bool xDir = (difX >= 0) ? XForward : !XForward;
    //      bool yDir = (difY >= 0) ? YForward : !YForward;
    //      difX = abs(difX);
    //      difY = abs(difY);
    //
    //      if (difX == 0) {
    //        stepY(yDir, difY);
    //        return;
    //      } else if (difY == 0) {
    //        stepX(xDir, difX);
    //        return;
    //      }
    //
    //      if (difX < difY) {
    //        int stepsForY = difY / difX;
    //        while (difX > 0 || difY > 0) {
    //          if (difY > 0) {
    //            stepY(yDir, stepsForY);
    //            difY -= stepsForY;
    //          }
    //          if (difX > 0) {
    //            stepX(xDir, 1);
    //            difX -= 1;
    //          }
    //        }
    //      } else {
    //        int stepsForX = difX / difY;
    //        while (difX > 0 || difY > 0) {
    //          if (difY > 0) {
    //            stepY(yDir, 1);
    //            difY -= 1;
    //          }
    //          if (difX > 0) {
    //            stepX(xDir, stepsForX);
    //            difX -= stepsForX;
    //          }
    //          stepX(xDir, stepsForX);
    //          stepY(yDir, 1);
    //          difX -= stepsForX;
    //          difY -= 1;
    //        }
    //      }
    //    }

    void lineToH(int x) {
      int difX =  x - this->currentX;
      this->currentX += difX;
      bool xDir = (difX >= 0) ? XForward : !XForward;
      difX = abs(difX);
      stepX(xDir, difX);
    }

    void lineToV(int y) {
      int difY =  y - this->currentY;
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

    void print() {
      Serial.print("x:");
      Serial.println(currentX);
      Serial.print("y:");
      Serial.println(currentY);
    }
};
