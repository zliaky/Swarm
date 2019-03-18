#ifndef __MOTOR__
#define __MOTOR__

#include "Arduino.h"
#include "Math.h"

#define PWM1 6
#define PWM2 7
#define PWM3 8
  
#define SWITCH1_1 22
#define SWITCH1_2 23
#define SWITCH2_1 24
#define SWITCH2_2 25
#define SWITCH3_1 26
#define SWITCH3_2 27

#define PWMMAX 255
#define VMAX 550  // 单个轮子实际速度的最大值

#define ANGLETR 3.1415926f / 180.0f
#define AFA 60  // 轮子方向与机器人坐标轴y轴角度为60

#define L 30.5 // 从机器人中心到轮子的距离，单位mm
#define theta 0 // 全局坐标系正方向与机器人坐标系正方向的夹角

class Motor {
  private:
  double Vx;
  double Vy;
  double angularVell;

  void ThreeWheelVellControl(void);  // Vx, Vy: 全局坐标速度，单位mm/s, angularVell: 角速度
  
  public:
  unsigned int pwm1;
  unsigned int pwm2;
  unsigned int pwm3;
  unsigned int dir1;
  unsigned int dir2;
  unsigned int dir3;
  
  double v1;
  double v2;
  double v3;

  void motorSetup(void);
  void motorMove(double vx, double vy, double vell);
  void moveToTmn(double Xc, double Yc, double Xt, double Yt);
};

#endif
