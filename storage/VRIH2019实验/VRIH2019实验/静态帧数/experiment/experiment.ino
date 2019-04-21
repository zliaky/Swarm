#include "OpticalData.h"

#define SCK_PIN 3
#define SDIO_PIN 4

OpticalData data(SCK_PIN,SDIO_PIN);

int startTime;
int currentTime;
int endTime;
int allCount = 0;
int correctCount = 0;
int hasPrinted = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  data.begin();
  startTime = millis();
  Serial.println("begin");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  data.CheckAndReadOpticalData();
  float Y=data.getYcoordinate();//读取Y坐标
  float X=data.getXcoordinate();//读取Y坐标
  allCount++;
  if (millis() - startTime < 10000) {
    if (Y > 0) {
      correctCount++;
/*      Serial.print(X, 4);
      Serial.print(", ");
      Serial.println(Y, 4);
  */  }
  } else if (hasPrinted < 5) {
    hasPrinted++;
    endTime = millis();
    Serial.print(endTime - startTime);
    Serial.print("ms, all Count: ");
    Serial.print(allCount);
    Serial.print(", correct Count: ");
    Serial.print(correctCount);
    Serial.print(", ratio: ");
    Serial.println((float)correctCount * 100 / allCount);
    startTime = millis();
    allCount = 0;
    correctCount = 0;
  }
}
