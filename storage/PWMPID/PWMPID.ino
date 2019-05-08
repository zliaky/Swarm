#include <MsTimer2.h>
#include "OpticalData.h"
#include "Motor.h"
#include "PID_v1.h"

#define beta 307 //机器人坐标系0°正方向与全局坐标系X轴正方向夹角
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
int countRotate;
boolean goOrnot=false;

double kp=2.5, ki=20, kd=1;
PID myPID(&Input, &Output, &Setpoint, kp, ki, kd, REVERSE);

#define SCK_PIN 21
#define SDIO_PIN 20

#define DIGITAL_LIGHT 34

#define LEN_CENTER 21

struct Stat {
  unsigned long Time;
  double X;
  double Y;
  int Angle;
};
struct motionMode{
  int modeOption;
  double X;
  double Y;
  int timeLength;
  int motionSpeed;
  }; 
  
OpticalData data(SCK_PIN, SDIO_PIN);
Stat cur, valid;
motionMode robotMotion;
Motor motor;

void setupPID() {
  Input = 0;
  Setpoint = 150;
  myPID.SetSampleTime(100);  //控制器的采样时间100ms
  //turn the PID on
    //myPID.SetTunings(kp,ki,kd);

   // myPID.SetOutputLimits(-60,60);
    myPID.SetMode(AUTOMATIC);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial3.begin(9600);
  data.begin();
  Setpoint=0;
  pinMode(DIGITAL_LIGHT, OUTPUT);
//  setupPID();
  MsTimer2::set(10,getData);
  MsTimer2::start();
  countRotate = 0;
  robotMotion.modeOption=2;
  robotMotion.timeLength=3;
  robotMotion.motionSpeed=100;
  robotMotion.X=815;
  robotMotion.Y=331;
  digitalWrite(DIGITAL_LIGHT, HIGH);
}

void dataRead() {
  data.CheckAndReadOpticalData();
  cur.Y = data.getYcoordinate(); //读取Y坐标
  cur.X = data.getXcoordinate(); //读取Y坐标
  cur.Angle = data.getAngle();
  Input = cur.Angle;
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
  for (int i = 0; i < 21; i++) {
    Serial3.write(str[i]);
  }
}

struct CenterFrame {
  char *start;        //1
  char *start1;       //1
  int *len;           //2
  int *id;            //2
  int *mode;          //2
  double *x;          //4
  double *y;          //4
  double *checkSum;   //4
  char *frameEnd;     //1
};
CenterFrame cFrame;
char cStr[LEN_CENTER];

struct DebugFrame {
  char start;
  char start1;
  long id;
  long dir[3];
  long pwm[3];
  long checkSum;
  char frameEnd;
};
DebugFrame dFrame;
char dStr[sizeof(DebugFrame)];

void recvFromCenter() {
  if (Serial3.available() >= sizeof(CenterFrame)) {
    char start = Serial3.read();
    char start1 = Serial3.read();
    if (start == '~' && start1 == '~') {
      cStr[0] = start;
      cStr[1] = start1;
      for (int i = 2; i < LEN_CENTER; i++) {
        cStr[i] = Serial3.read();
      }
  
      char *p = cStr;
      cFrame.start = p;
      cFrame.start1 = (char*)(p = p + sizeof(char));
      cFrame.len = (int*)(p = p + sizeof(char));
      cFrame.id = (int*)(p = p + sizeof(int));
      cFrame.mode = (int*)(p = p + sizeof(int));
      cFrame.x = (double*)(p = p + sizeof(int));
      cFrame.y = (double*)(p = p + sizeof(double));
      cFrame.checkSum = (double*)(p = p + sizeof(double));
      cFrame.frameEnd = (char*)(p = p + sizeof(double));
  
      if (*cFrame.frameEnd == '!'&& (*cFrame.checkSum == (*cFrame.x + *cFrame.y))&& *cFrame.id == 1) {
        // 
        digitalWrite(DIGITAL_LIGHT, LOW);
        robotMotion.modeOption = *cFrame.mode;
        robotMotion.X = *cFrame.x;
        robotMotion.Y = *cFrame.y;
      }
    }
  }
}
int count=0;
void getData(){
  dataRead();
  if (cur.X > 5 && cur.Y > 5 && cur.X < 1500 && cur.Y < 600) {
     valid=cur;
     serialPrint(1, valid.X, valid.Y, valid.Angle);
  }
  Serial3.flush();
 }

void loop() { 
  //motor.motorMove_2(0,250,140,0,1,0);
  displayMode();
  recvFromCenter();
  delay(10);
}

int deltaScale=20;
void checkPosition(double destinationX, double destinationY){
     double Xscale_max=destinationX+deltaScale;
     double Yscale_max=destinationY+deltaScale;
     double Xscale_min=destinationX-deltaScale;
     double Yscale_min=destinationY-deltaScale;
     if((valid.X>Xscale_min)&&(valid.X<Xscale_max)&&(valid.Y>Yscale_min)&&(valid.Y<Yscale_max)){
            motor.goStop();
            goOrnot=false;
          //  delay(10000); 
           // robotMotion.modeOption=0;  
        }
      else 
        goOrnot=true;
    }
int rotatedir=0;
int rotatePWM=0;
void controlDirection(int curAngle) {
  rotatePWM=abs(Setpoint-curAngle);
  if(rotatePWM>3){
      countRotate++;
      if(curAngle<Setpoint&&rotatePWM<180 ){
          rotatedir=1;   
      }
      else if(curAngle>=Setpoint&&rotatePWM<180){
          rotatedir=-1;  
      }
      else if(curAngle<Setpoint&&rotatePWM>180){
          rotatedir=-1; 
          rotatePWM=360-rotatePWM;
        }
      else if(curAngle>=Setpoint&&rotatePWM>180){
          rotatedir=1;
          rotatePWM=360-rotatePWM;
      }
    }
   if(rotatePWM>40)
      rotatePWM=40;    
   if(rotatePWM<=3){
      countRotate=0;
      for(int i=0;i<180;i++){
        motor.motorMove_2(0,250,130,0,1,0);
        checkPosition(robotMotion.X, robotMotion.Y);
        if(!goOrnot)
            break;
        delay(10);
      }
      controlDestination(robotMotion.X, robotMotion.Y,valid.X,valid.Y);    
      }     
   if(countRotate>2){
       motor.rotate(rotatedir,rotatePWM);
       rotatedir=0;
    }
   else{
      motor.goStop();
    }
    checkPosition(robotMotion.X, robotMotion.Y);
}

void controlDestination(double aimX, double aimY, double curX, double curY){
  int alpha=(int)360*atan((aimY-curY)/(aimX-curX))/(2*PI);
  if(aimY<curY){
    if(alpha>0){
      Setpoint=beta-alpha-180;
      }
    else{
       Setpoint=beta-alpha;
      }
    }
   else if(aimY>curY){
    if(alpha>0){
      Setpoint=beta-alpha;
      }
    else{
       Setpoint=beta-alpha-180;
      }
    }
  }



void rotateMode(int timeLength){
 // int Ncount=timeLength*1000/5;
 // for(int i=0;i<Ncount;i++){
    motor.rotateMotion(0,100);
   // delay(5);
   // }
   //motor.goStop();
  }

void displayMode(){
  switch(robotMotion.modeOption){
    case 0:               //原地静止不动
      motor.goStop();
      break;
      
    case 1:               //原地旋转
      rotateMode(robotMotion.timeLength);
     // robotMotion.modeOption=0;
      break;

    case 2:            //直线运动
       checkPosition(robotMotion.X, robotMotion.Y);
       if(goOrnot){
       controlDirection(valid.Angle);
       }       
       break;
    }
  }
