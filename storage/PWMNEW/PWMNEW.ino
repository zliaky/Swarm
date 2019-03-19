#include "OpticalData.h"

#define SCK_PIN 21
#define SDIO_PIN 20

#define PWM1 5
#define PWM2 2
#define PWM3 3

#define SWITCH1_1 22
#define SWITCH1_2 23
#define SWITCH2_1 24
#define SWITCH2_2 25
#define SWITCH3_1 26
#define SWITCH3_2 27

struct Stat {
  unsigned long Time;
  double X;
  double Y;
  int Angle;
};

OpticalData data(SCK_PIN,SDIO_PIN);
Stat Begin, last, cur;
bool first;
float totalDis, avrSpeed;
float curDis, curSpeed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial3.begin(9600);
  data.begin();
  pinMode(SWITCH1_1, OUTPUT);
  pinMode(SWITCH1_2, OUTPUT);
  pinMode(SWITCH2_1, OUTPUT);
  pinMode(SWITCH2_2, OUTPUT);
  pinMode(SWITCH3_1, OUTPUT);
  pinMode(SWITCH3_2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  first = true;
  totalDis = 0;
}

void motorMove() {
  digitalWrite(SWITCH1_1, 0);
  digitalWrite(SWITCH1_2, 1);
  digitalWrite(SWITCH2_1, 0);
  digitalWrite(SWITCH2_2, 1);
  digitalWrite(SWITCH3_1, 0);
  digitalWrite(SWITCH3_2, 1);
  analogWrite(PWM1, 128);
  analogWrite(PWM2, 128);
  analogWrite(PWM3, 0);
}

void dataRead() {
  data.CheckAndReadOpticalData();
  cur.Y = data.getYcoordinate(); //读取Y坐标
  cur.X = data.getXcoordinate(); //读取Y坐标
  cur.Angle = data.getAngle();
  cur.Time = millis();
}

void firstCoor() {
  if (!first) {
    return;
  } else {
    Begin.X = cur.X;
    Begin.Y = cur.Y;
    Begin.Time = cur.Time;
    last.X = cur.X;
    last.Y = cur.Y;
    last.Time = cur.Time;
    first = false;
  }
}

float calcDis(float x1, float y1, float x2, float y2) {
  return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void printInfo() {
    Serial.print("current distance: ");
    Serial.print(curDis);
    Serial.print(", current speed: ");
    Serial.print(curSpeed);
    Serial.print(", total distance: ");
    Serial.print(totalDis);
    Serial.print(", average speed: ");
    Serial.println(avrSpeed);
}

struct Frame {
  char start;   //1
  char start1;  //1
  int len;      //2
  int id;       //2
  double x;     //4
  double y;     //4
  int angle;    //2
  double checkSum;   //4
  char frameEnd;     //1
};
Frame frame;
char *str;

void serialPrint(int id, double x, double y, int angle) {
  frame.start = '~';
  frame.start1 = '~';
  frame.len = 48;
  frame.id = id;
  frame.x = x;
  frame.y = y;
  frame.angle = angle;
  frame.checkSum = frame.x + frame.y;
  frame.frameEnd = '!';
  str = (char*)&frame;
  for (int i = 0; i < 21; i++) {
    Serial3.write(str[i]);
  }
}


void loop() {
 // motorMove();
    dataRead();
    if(cur.X!=-1){
      serialPrint(0, cur.X, cur.Y, cur.Angle);
   }
   delay(100);
}
