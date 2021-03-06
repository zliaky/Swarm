#include "OpticalData.h"

#define SCK_PIN 3
#define SDIO_PIN 4

OpticalData data(SCK_PIN,SDIO_PIN);

int startTime;
int currentTime;
int endTime;
int correctCount = 0;
int hasPrinted = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  data.begin();
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  data.CheckAndReadOpticalData();
  float Y=data.getYcoordinate();//读取Y坐标
  float X=data.getXcoordinate();//读取Y坐标
  if (hasPrinted < 3) {
    if (millis() - startTime < 10000) {
      if (Y > 1 && X > 1 && X < 1074 && Y < 601) {
        correctCount++;
        Serial.print(millis()-startTime);
        Serial.print(",");
        Serial.print(X, 4);
        Serial.print(",");
        Serial.println(Y, 4);
      }
    } else {
      hasPrinted++;
      endTime = millis();
      Serial.print(endTime - startTime);
      Serial.print("ms, correct Count: ");
      Serial.println(correctCount);
      startTime = millis();
      correctCount = 0;
    }
  }
}
