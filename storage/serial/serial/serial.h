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
	char *start;		//验证位1
	char *start1;		//验证位2
	short *len;			//数据长度，暂时没用上
	short *id;			//机器人id
	float *x;			//机器人坐标
	float *y;			//机器人坐标
	short *angle;		//机器人角度
	float *checkSum;    //校验和
	char *frameEnd;     //包尾验证位
}Frame;

class Serial {
private:
	DWORD btsIO;
	DCB lpTest;
	HANDLE hDevice;
	char sendText[LEN];
	char recvText[LEN];
public:
	Frame sendF;
	Frame recvF;
	
	bool initSerial();
	void serialLoop();

	void sendFrame();
	void recvFrame();
};