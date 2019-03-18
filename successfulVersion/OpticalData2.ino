#include "OpticalData.h"
#include "Motor.h"
#include "Serial.h"


#define SCK_PIN 3
#define SDIO_PIN 4

OpticalData data(SCK_PIN,SDIO_PIN);
Motor motor;
Frame recvF, sendF;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motor.motorSetup();
  data.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  data.CheckAndReadOpticalData();
  
  double Xc = data.getXcoordinate();
  double Yc = data.getYcoordinate();
  if (Yc > 0) {
  Serial.print(Xc);
  Serial.print(", ");
  Serial.println(Yc);
    
  }
//  motor.motorMove(10, 8, 0);

//  printInfo();
}

void serialIO() {
  String str = "";
  while (Serial.available() > 0) {
    str = str + Serial.read();
  }
  if (str.length() > 0) {
//    这里从str转Frame不知道怎么转，先放着吧    
//    memcpy(&recvF, str, sizeof(recvF));
    byte b[sizeof(Frame)];
    memcpy(b, &sendF, sizeof(Frame));
    
    Serial.write(b, sizeof(Frame));
  }
  delay(1000);
}

void printInfo() {
  Serial.println("=======");
  Serial.print("pwm: ");
  Serial.print(motor.pwm1);
  Serial.print(", ");
  Serial.print(motor.pwm2);
  Serial.print(", ");
  Serial.println(motor.pwm3);
  Serial.print("dir: ");
  Serial.print(motor.dir1);
  Serial.print(", ");
  Serial.print(motor.dir2);
  Serial.print(", ");
  Serial.println(motor.dir3);
  Serial.print("v: ");
  Serial.print(motor.v1);
  Serial.print(", ");
  Serial.print(motor.v2);
  Serial.print(", ");
  Serial.println(motor.v3);
}
