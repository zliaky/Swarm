#include "Motor.h"

void Motor::motorSetup(void) {
  pinMode(SWITCH1_1, OUTPUT);
  pinMode(SWITCH1_2, OUTPUT);
  pinMode(SWITCH2_1, OUTPUT);
  pinMode(SWITCH2_2, OUTPUT);
  pinMode(SWITCH3_1, OUTPUT);
  pinMode(SWITCH3_2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
}

/*
 * 将机器人的全局运动速度映射到三个电机速度上
 * v1、v2、v3：三个电机的线速度
 * https://blog.csdn.net/jyaxp/article/details/55050393
 */
void Motor::rotate(int clockwise, int rotatespeed){
  int speedRatio1=27;
  int speedRatio2=27;
  if(clockwise<0){
      motorMove_2(rotatespeed+speedRatio1,rotatespeed+speedRatio1,rotatespeed+speedRatio1,0,0,0);
    }
  else{
     if(rotatespeed<15){
        speedRatio2=60;
     }
      motorMove_2(rotatespeed+speedRatio2,rotatespeed+speedRatio2,rotatespeed+speedRatio2,1,1,1);
    } 
  } 
void Motor::rotateMotion(int clockwise,int rotatespeed){
  if(clockwise<0){
      motorMove_2(rotatespeed,rotatespeed,rotatespeed,0,0,0);
    }
  else{
      motorMove_2(rotatespeed,rotatespeed,rotatespeed,1,1,1);
    } 
  }
void Motor::goStop(){
  motorMove_2(0, 0, 0, 0, 0, 0);
 // delay(1000);
  }
void Motor::motorMove(double vx, double vy, double vell) {
  
  pwm1 = int(abs(v1 / VMAX * PWMMAX));
  pwm2 = int(abs(v2 / VMAX * PWMMAX));
  pwm3 = int(abs(v3 / VMAX * PWMMAX));

  dir1 = (v1 > 0) ? 1 : 0;
  dir2 = (v2 > 0) ? 1 : 0;
  dir3 = (v3 > 0) ? 1 : 0;
    
  digitalWrite(SWITCH1_1, dir1);
  digitalWrite(SWITCH1_2, 1-dir1);
  digitalWrite(SWITCH2_1, dir2);
  digitalWrite(SWITCH2_2, 1-dir2);
  digitalWrite(SWITCH3_1, 1-dir3);
  digitalWrite(SWITCH3_2, dir3);
  analogWrite(PWM1, pwm1);
  analogWrite(PWM2, pwm2);
  analogWrite(PWM3, pwm3);
}

void Motor::moveToTmn(double Xc, double Yc,int Angle_c, double Xt, double Yt) {
  double s = sqrt((Yt-Yc)*(Yt-Yc) + (Xt-Xc)*(Xt-Xc));
  double alpha = atan((Yt-Yc) / (Xt-Xc));
  double Vx = VMAX / 2 * cos(alpha);
  double Vy = VMAX / 2 * sin(alpha);
  motorMove(Vx, Vy, Angle_c);
}
void Motor::motorMove_2(int a, int b, int c,int a_d,int b_d, int c_d) {

  pwm1 = a;
  pwm2 = b;
  pwm3 = c;

  dir1 = a_d;
  dir2 = b_d;
  dir3 = c_d;
    
  digitalWrite(SWITCH1_1, dir1);
  digitalWrite(SWITCH1_2, 1-dir1);
  digitalWrite(SWITCH2_1, dir2);
  digitalWrite(SWITCH2_2, 1-dir2);
  digitalWrite(SWITCH3_1, dir3);
  digitalWrite(SWITCH3_2, 1-dir3);
  analogWrite(PWM1, pwm1);
  analogWrite(PWM2, pwm2);
  analogWrite(PWM3, pwm3);
}
