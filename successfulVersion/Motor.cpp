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
void Motor::ThreeWheelVellControl(void) {
  
  v1 = (double)(-cos((AFA + theta) * ANGLETR) * Vx - sin((theta + AFA) * ANGLETR) * Vy + L * angularVell);
  
  v2 = (double)(cos(theta * ANGLETR) * Vx + sin(theta * ANGLETR) * Vy + L * angularVell);
  
  v3 = (double)(-cos((AFA - theta) * ANGLETR) * Vx + sin((AFA - theta) * ANGLETR) * Vy + L * angularVell);
}

/*
 * 从三个电机速度得到应给出的PWM波并写入开发板中
 */
void Motor::motorMove(double vx, double vy, double vell) {

  Vx = vx;
  Vy = vy;
  angularVell = vell;

  ThreeWheelVellControl();
  
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
  digitalWrite(SWITCH3_1, dir3);
  digitalWrite(SWITCH3_2, 1-dir3);
  analogWrite(PWM1, pwm1);
  analogWrite(PWM2, pwm2);
  analogWrite(PWM3, pwm3);
}

void Motor::moveToTmn(double Xc, double Yc, double Xt, double Yt) {
  double s = sqrt((Yt-Yc)*(Yt-Yc) + (Xt-Xc)*(Xt-Xc));
  double alpha = atan((Yt-Yc) / (Xt-Xc));
  double Vx = VMAX / 2 * cos(alpha);
  double Vy = VMAX / 2 * sin(alpha);
  motorMove(Vx, Vy, 0);
}
