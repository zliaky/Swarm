#pragma once

#include "windows.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <conio.h>
#include <sstream>
using namespace std;

#define LEN 21

typedef struct {
	char start;		//验证位1
	char start1;		//验证位2
	short len;			//数据长度，暂时没用上
	short id;			//机器人id
	float x;			//机器人坐标
	float y;			//机器人坐标
	short angle;		//机器人角度
	float checkSum;    //校验和
	char frameEnd;     //包尾验证位
}Frame;
/*
typedef struct {
	char start;		//验证位1
	char start1;		//验证位2
	short len;			//数据长度，暂时没用上
	short id;			//机器人id
	float x;			//目标坐标
	float y;
	float vx;			//线速度
	float vy;
	short dA;			//需要转动的角度
	float angV;			//角速度
	float checkSum;    //校验和
	char frameEnd;     //包尾验证位
}SFrame;
*/
typedef struct {
	char start;		//验证位1
	char start1;		//验证位2
	short len;			//数据长度，暂时没用上
	short id;			//机器人id
	short mode;
	float x;			//目标坐标
	float y;
	float checkSum;    //校验和
	char frameEnd;     //包尾验证位
}SFrame;

typedef struct {
	char start;
	char start1;
	short id;
	short dir0;
	short dir1;
	short dir2;
	short pwm0;
	short pwm1;
	short pwm2;
	short checkSum;
	char frameEnd;
}DFrame;

class Serial {
private:
	DWORD btsIO;
	DCB lpTest;
	HANDLE hDevice;
	char recvText[LEN];
	DWORD error;
public:
	short id;
	float x, y;
	short angle;

	bool initSerial(char* com);
	int serialClose();

	void sendFrame(short id, short mode, float x, float y);
	void sendDebug(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3);
	void recvFrame();
};