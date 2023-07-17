#include "Lazer.h"
#include "CubicBezier.h"

#define DIR_PIN_X 2
#define STEPPER_PIN_X 3
#define DIR_PIN_Y 4
#define STEPPER_PIN_Y 5
#define CONNECT_PIN 13
#define START_PIN A1
#define VOLTAGE_PIN A2

CubicBezier cb = CubicBezier();
Vector v = Vector(0, 0);
char comand;

const Lazer lazer = Lazer(DIR_PIN_X, STEPPER_PIN_X, DIR_PIN_Y, STEPPER_PIN_Y);

void setup() {
  pinMode(DIR_PIN_X, OUTPUT);
  pinMode(STEPPER_PIN_X, OUTPUT);
  pinMode(DIR_PIN_Y, OUTPUT);
  pinMode(STEPPER_PIN_Y, OUTPUT);
  pinMode(CONNECT_PIN, OUTPUT);
  pinMode(START_PIN, OUTPUT);
  pinMode(VOLTAGE_PIN, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(40);
  analogReference(INTERNAL);
}

const float R2 = 4.9, Vinternal = 1.055;
const int R1 = 99, p = 1;
int sum = 0;

void loop() {
//  sum = 0;
//  for (int i = 0; i < p; i++) {
//    sum += analogRead(VOLTAGE_PIN);
//  }
//  float variable = ((sum / p) * Vinternal) / 1023;
//  variable = variable / (R2 / (R1 + R2));
//  Serial.println(variable);


  if (Serial.available()) {
    comand = (char)Serial.read();
    switch (comand) {
      case 'I': {
          makeInstruction(Serial.readString());
          break;
        }
      case 'M': {
          digitalWrite(START_PIN, HIGH);
          int x = Serial.parseInt();
          int y = Serial.parseInt();
          lazer.lineTo(x, y);
          digitalWrite(12, HIGH);
          delay(50);
          Serial.write('O');
          break;
        }
      case 'L': {
          int x = Serial.parseInt();
          int y = Serial.parseInt();
          lazer.lineTo(x, y);
          Serial.write('O');
          break;
        }
      case 'H': {
          lazer.lineToH(Serial.parseInt());
          Serial.write('O');
          break;
        }
      case 'V': {
          lazer.lineToV(Serial.parseInt());
          Serial.write('O');
          break;
        }

      case 'C': {
          int x1 = Serial.parseInt();
          int y1 = Serial.parseInt();
          int x2 = Serial.parseInt();
          int y2 = Serial.parseInt();
          int endX = Serial.parseInt();
          int endY = Serial.parseInt();
          cb.setFactors(x1, y1, x2, y2);
          cb.setPoints(lazer.getX(), lazer.getY(), endX, endY);
          for (float t = 0.0; t <= 1; t += 0.1) {
            v = cb.getPoint(t);
            //            Serial.print("x:");
            //            Serial.println(v.x);
            //            Serial.print("y:");
            //            Serial.println(v.y);
            lazer.lineTo(v.x, v.y);
          }
          v = cb.getPoint(1);
          //          Serial.print("x:");
          //          Serial.println(v.x);
          //          Serial.print("y:");
          //          Serial.println(v.y);
          lazer.lineTo(v.x, v.y);
          Serial.write('O');
          break;
        }

      case 'Z': {
          int x = Serial.parseInt();
          int y = Serial.parseInt();
          lazer.lineTo(x, y);
          digitalWrite(START_PIN, LOW);
          digitalWrite(12, LOW);
          Serial.write('O');
          break;
        }
      case 'E': {
          digitalWrite(12, LOW);
          lazer.reset();
          digitalWrite(START_PIN, LOW);
          Serial.write('O');
          break;
        }
    }
    clearBuff();
  }
}

void makeInstruction(String comandLine) {
  if (comandLine.indexOf(" UC") != -1) {
    digitalWrite(CONNECT_PIN, LOW);
    Serial.write('O');
  } else if (comandLine.indexOf(" C") != -1) {
    digitalWrite(CONNECT_PIN, HIGH);
    Serial.write('O');
  }
  if (comandLine.indexOf(" R") != -1) {
    lazer.reset();
    digitalWrite(START_PIN, LOW);
    Serial.write('O');
  }
}

void clearBuff() {
  while (Serial.available()) {
    Serial.read();
  }
}