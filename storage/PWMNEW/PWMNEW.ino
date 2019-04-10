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
#define LEN_DEBUG 19
#define LEN_SEND 21

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
  analogWrite(PWM1, 250);
  analogWrite(PWM2, 250);
  analogWrite(PWM3, 0);
}

void dataRead() {
  data.CheckAndReadOpticalData();
  cur.Y = data.getYcoordinate(); //读取Y坐标
  cur.X = data.getXcoordinate(); //读取Y坐标
  cur.Angle = data.getAngle();
  cur.Time = millis();
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
  for (int i = 0; i < LEN_SEND; i++) {
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

struct DebugFrame {
  char start;
  char start1;
  int id;
  int dir0;
  int dir1;
  int dir2;
  int pwm0;
  int pwm1;
  int pwm2;
  int checkSum;
  char frameEnd;
};
DebugFrame dFrame;
char dStr[LEN_DEBUG];

void debugMove() {
  digitalWrite(SWITCH1_1, dFrame.dir0);
  digitalWrite(SWITCH1_2, 1-dFrame.dir0);
  digitalWrite(SWITCH2_1, dFrame.dir1);
  digitalWrite(SWITCH2_2, 1-dFrame.dir1);
  digitalWrite(SWITCH3_1, dFrame.dir2);
  digitalWrite(SWITCH3_2, 1-dFrame.dir2);
  analogWrite(PWM1, dFrame.pwm0);
  analogWrite(PWM2, dFrame.pwm1);
  analogWrite(PWM3, dFrame.pwm2);
}

void recvFromCenter() {
  char start = Serial3.read();
  char start1 = Serial3.read();
  if (start == '~' && start1 == '`') {
    cStr[0] = start;
    cStr[1] = start1;  
    for (int i = 2; i < LEN_CENTER; i++) {
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
    
    if (*cFrame.frameEnd == '!' && (*cFrame.checkSum == (*cFrame.x + *cFrame.y))) {
    }
  } else if (start == 'D' && start1 == 'e') {
    dStr[0] = start;
    dStr[1] = start1;
    for (int i = 2; i < LEN_DEBUG; i++) {
      dStr[i] = Serial3.read();
    }
    char *p = dStr;
    dFrame.start = *p;
    dFrame.start1 = *((char*)(p=p+sizeof(char)));
    dFrame.id = *((int*)(p=p+sizeof(char)));
    dFrame.dir0 = *((int*)(p=p+sizeof(int)));
    dFrame.dir1 = *((int*)(p=p+sizeof(int)));
    dFrame.dir2 = *((int*)(p=p+sizeof(int)));
    dFrame.pwm0 = *((int*)(p=p+sizeof(int)));
    dFrame.pwm1 = *((int*)(p=p+sizeof(int)));
    dFrame.pwm2 = *((int*)(p=p+sizeof(int)));
    dFrame.checkSum = *((int*)(p=p+sizeof(int)));
    dFrame.frameEnd = *((char*)(p=p+sizeof(int)));
    if (dFrame.frameEnd == '!' && (dFrame.checkSum == (dFrame.dir0 + dFrame.pwm0))) {
      debugMove();
    }
  }
}

void loop() {
  dataRead();
 if(cur.X>1 && cur.Y>5&&cur.X<1500&&cur.Y<600){
    last=cur;
    serialPrint(3, cur.X, cur.Y, cur.Angle);
  }
//  else
//    serialPrint(3, last.X, last.Y, last.Angle);
  recvFromCenter();

  Serial3.flush();
  delay(50);
}
