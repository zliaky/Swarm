#include "OpticalData.h"

#define SCK_PIN 3
#define SDIO_PIN 4

OpticalData data(SCK_PIN,SDIO_PIN);


int cnt;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  data.begin();
  cnt = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  data.CheckAndReadOpticalData();
  float Y=data.getYcoordinate();//读取Y坐标
  float X=data.getXcoordinate();//读取Y坐标
  if(Y>0){
    if (cnt < 100)
    {
      Serial.print(X, 4);
      Serial.print(", ");
      Serial.println(Y, 4);
      cnt++;
    }
  }
}
