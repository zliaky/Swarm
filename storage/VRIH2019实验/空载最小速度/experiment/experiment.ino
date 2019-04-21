#include "OpticalData.h"

#define SCK_PIN 3
#define SDIO_PIN 4

OpticalData data(SCK_PIN,SDIO_PIN);

float lastX=0, lastY=0;
#define threshold 1
int allCount = 0;
int yCount = 0;
int nCount = 0;
bool hasPrinted = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  data.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  data.CheckAndReadOpticalData();
  float Y=data.getYcoordinate();//读取Y坐标
  float X=data.getXcoordinate();//读取Y坐标
  if (allCount < 21) {
    if (Y > 300 && X > 4 && X < 250 && Y < 620) {
      if (abs(Y - lastY) > threshold || abs(X - lastX) > threshold) {
        Serial.println("Y"); 
        yCount++;
      } else {
        Serial.println("N");
        nCount++;
      }
      allCount++;
      lastY = Y;
      lastX = X;
/*    Serial.print(X, 4);
    Serial.print(", ");
    Serial.println(Y, 4);*/
    }
  } else {
    if (!hasPrinted) {
        
      hasPrinted = true;
      Serial.print("allCount: ");
      Serial.print(allCount-1);
      Serial.print(", yCount: ");
      Serial.print(yCount-1);
      Serial.print(", nCount: ");
      Serial.println(nCount);
    }
  }
}
