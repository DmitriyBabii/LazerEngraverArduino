#include "Lazer.h"
#include "CubicBezier.h"

#define DIR_PIN_X 2
#define STEPPER_PIN_X 3
#define DIR_PIN_Y 4
#define STEPPER_PIN_Y 5
#define CONNECT_PIN 13
#define START_PIN A1
#define VOLTAGE_PIN A2
#define LAZER_PIN 12

const Lazer lazer = Lazer(DIR_PIN_X, STEPPER_PIN_X, DIR_PIN_Y, STEPPER_PIN_Y);
double accuracy = conf::cubicPrecision;

void setup() {
  // register pins
  pinMode(DIR_PIN_X, OUTPUT);
  pinMode(STEPPER_PIN_X, OUTPUT);
  pinMode(DIR_PIN_Y, OUTPUT);
  pinMode(STEPPER_PIN_Y, OUTPUT);
  pinMode(CONNECT_PIN, OUTPUT);
  pinMode(START_PIN, OUTPUT);
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(LAZER_PIN, OUTPUT);

  // start serial and set parameters
  Serial.begin(9600);
  Serial.setTimeout(20);
}

void loop() {
}

void serialEvent() {
  char comand = (char)Serial.read();
  switch (comand) {
    case 'I':
      {
        makeInstruction(Serial.readString());
        break;
      }
    case 'M':
      {
        digitalWrite(START_PIN, HIGH);
        int x = Serial.parseInt();
        int y = Serial.parseInt();
        lazer.start(x, y);
        digitalWrite(LAZER_PIN, HIGH);
        delay(50);
        Serial.write('O');
        break;
      }
    case 'L':
      {
        int x = Serial.parseInt();
        int y = Serial.parseInt();
        lazer.lineTo(x, y);
        Serial.write('O');
        break;
      }
    case 'H':
      {
        lazer.lineToH(Serial.parseInt());
        Serial.write('O');
        break;
      }
    case 'V':
      {
        lazer.lineToV(Serial.parseInt());
        Serial.write('O');
        break;
      }

    case 'C':
      {
        int x1 = Serial.parseInt();
        int y1 = Serial.parseInt();
        int x2 = Serial.parseInt();
        int y2 = Serial.parseInt();
        int endX = Serial.parseInt();
        int endY = Serial.parseInt();

        CubicBezier cb = CubicBezier();
        Vector v = Vector(0, 0);
        cb.setFactors(x1, y1, x2, y2);
        cb.setPoints(lazer.getX(), lazer.getY(), endX, endY);
        for (float t = 0.0; t <= 1.0; t += accuracy) {
          t = roundToDecimalPlaces(t, 2);
          v = cb.getPoint(t);
          lazer.lineTo(v.x, v.y);
        }

        // shouldnt be here
        // v = cb.getPoint(1);
        // lazer.lineTo(v.x, v.y);

        Serial.write('O');
        break;
      }
    case 'Z':
      {
        lazer.loopLine();
        digitalWrite(START_PIN, LOW);
        digitalWrite(LAZER_PIN, LOW);
        Serial.write('O');
        break;
      }
    case 'E':
      {
        digitalWrite(LAZER_PIN, LOW);
        Serial.write('O');
        break;
      }
    case 'R':
      {
        digitalWrite(LAZER_PIN, LOW);
        lazer.reset();
        digitalWrite(START_PIN, LOW);
        Serial.write('O');
        break;
      }
  }
  //clearBuff();
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
    digitalWrite(LAZER_PIN, LOW);
    lazer.reset();
    digitalWrite(START_PIN, LOW);
    Serial.write('O');
  }

  int parameterValueStart;
  if ((parameterValueStart = comandLine.indexOf("A ")) != -1) {
    parameterValueStart += 2;
    String parameterValueStr = comandLine.substring(parameterValueStart);
    accuracy = strtod(parameterValueStr.c_str(), NULL);
    Serial.write('O');
  }
  if ((parameterValueStart = comandLine.indexOf("S ")) != -1) {
    parameterValueStart += 2;
    String parameterValueStr = comandLine.substring(parameterValueStart);
    lazer.setSpeed(parameterValueStr.toInt());
    Serial.write('O');
  }

  clearBuff();
}

void clearBuff() {
  while (Serial.available()) {
    Serial.read();
  }
}

float roundToDecimalPlaces(float number, int decimalPlaces) {
  float multiplier = pow(10.0, decimalPlaces);
  return round(number * multiplier) / multiplier;
}
