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

#define DIGITAL_LIGHT 34

#define LEN_CENTER 33

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
  pinMode(DIGITAL_LIGHT, OUTPUT);
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
  analogWrite(PWM1, 150);
  analogWrite(PWM2, 150);
  analogWrite(PWM3, 150);
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

struct CenterFrame {
  char *start;        //1
  char *start1;       //1
  int *len;           //2
  int *id;            //2
  double *x;          //4
  double *y;          //4
  double *vx;         //4
  double *vy;         //4
  int *dA;            //2
  double *angV;       //4
  double *checkSum;   //4
  char *frameEnd;     //1
};
CenterFrame cFrame;
char cStr[LEN_CENTER];

void recvFromCenter() {
  for (int i = 0; i < LEN_CENTER; i++) {
    cStr[i] = Serial3.read();
  }

  char *p = cStr;
  cFrame.start = p;
  cFrame.start1 = (char*)(p=p+sizeof(char));
  cFrame.len = (int*)(p=p+sizeof(char));
  cFrame.id = (int*)(p=p+sizeof(int));
  cFrame.x = (double*)(p=p+sizeof(int));
  cFrame.y = (double*)(p=p+sizeof(double));
  cFrame.vx = (double*)(p=p+sizeof(double));
  cFrame.vy = (double*)(p=p+sizeof(double));
  cFrame.dA = (int*)(p=p+sizeof(double));
  cFrame.angV = (double*)(p=p+sizeof(int));
  cFrame.checkSum = (double*)(p=p+sizeof(double));
  cFrame.frameEnd = (char*)(p=p+sizeof(double));
}

void loop() {
//    motorMove();
  dataRead();
  if(cur.X!=-1 && cur.Y!=-1){
    serialPrint(0, cur.X, cur.Y, cur.Angle);
  }
  recvFromCenter();
  if (*cFrame.start == '~' && *cFrame.start1 == '~' && *cFrame.frameEnd == '!' && (*cFrame.checkSum == (*cFrame.x + *cFrame.y))) {
    digitalWrite(DIGITAL_LIGHT, HIGH);
    delay(100);
    digitalWrite(DIGITAL_LIGHT, LOW);
  }
  Serial3.flush();
  delay(50);
}
