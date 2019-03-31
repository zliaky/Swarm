#pragma once

#include "windows.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <conio.h>
#include <sstream>
using namespace std;

#define LEN 21
#define SEND_LEN 33

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

class Serial {
private:
	DWORD btsIO;
	DCB lpTest;
	HANDLE hDevice;
	char recvText[LEN];
	SFrame sendF;
	DWORD error;
public:
	short id;
	float x, y;
	short angle;

	bool initSerial(char* com);
	int serialClose();

	void sendFrame(short id, float x, float y, float vx, float vy, short dA, float angV);
	void recvFrame();
};