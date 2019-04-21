#include "OpticalData.h"

#define SCK_PIN 3
#define SDIO_PIN 4

OpticalData data(SCK_PIN,SDIO_PIN);
int lastTime;
int startTime;
float lastX = 0, lastY = 0;
int first = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  data.begin();
  lastTime = millis();
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  data.CheckAndReadOpticalData();
  float Y=data.getYcoordinate();//读取Y坐标
  float X=data.getXcoordinate();//读取Y坐标
  if (Y > 1 && X > 1 && X < 1074 && Y < 601) {
    int currentTime = millis();
    if (!first) {
      Serial.print(currentTime-startTime);
      Serial.print(", ");
      Serial.print(X, 4);
      Serial.print(", ");
      Serial.print(Y, 4);
      Serial.print(", ");
      Serial.print(abs(X-lastX)/(currentTime-lastTime));
      Serial.print(", ");
      Serial.println(abs(Y-lastY)/(currentTime-lastTime));
    } else {
      first = false;
    }
    lastX = X;
    lastY = Y;
    lastTime = currentTime;
  }
}
