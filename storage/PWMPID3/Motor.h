#ifndef __MOTOR__
#define __MOTOR__

#include "Arduino.h"
#include "Math.h"

#define PWM1 5
#define PWM2 2
#define PWM3 3

#define SWITCH1_1 24
#define SWITCH1_2 25
#define SWITCH2_1 26
#define SWITCH2_2 27
#define SWITCH3_1 22
#define SWITCH3_2 23

#define PWMMAX 255
#define VMAX 550  // 单个轮子实际速度的最大值

#define ANGLETR 3.1415926f / 180.0f
#define AFA 60  // 轮子方向与机器人坐标轴y轴角度为60

#define L 30.5 // 从机器人中心到轮子的距离，单位mm
#define theta 55 // 全局坐标系x正方向与机器人坐标系x正方向的夹角

//#define rotateSpeed 100//机器人旋转速度
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
  void rotate(int clockwise,int rotatespeed);
  void rotateMotion(int clockwise,int rotatespeed);
  void goStop();
  void motorMove(double vx, double vy, double vell);
  void motorMove_2(int a, int b, int c,int a_d,int b_d, int c_d);
  void moveToTmn(double Xc, double Yc,int Angle_c, double Xt, double Yt);
};

#endif
