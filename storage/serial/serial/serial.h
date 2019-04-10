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
	char start;		//��֤λ1
	char start1;		//��֤λ2
	short len;			//���ݳ��ȣ���ʱû����
	short id;			//������id
	float x;			//����������
	float y;			//����������
	short angle;		//�����˽Ƕ�
	float checkSum;    //У���
	char frameEnd;     //��β��֤λ
}Frame;

typedef struct {
	char start;		//��֤λ1
	char start1;		//��֤λ2
	short len;			//���ݳ��ȣ���ʱû����
	short id;			//������id
	float x;			//Ŀ������
	float y;
	float vx;			//���ٶ�
	float vy;
	short dA;			//��Ҫת���ĽǶ�
	float angV;			//���ٶ�
	float checkSum;    //У���
	char frameEnd;     //��β��֤λ
}SFrame;

typedef struct {
	char start;
	char start1;
	int id;
	int dir[3];
	int pwm[3];
	int checkSum;
	char frameEnd;
}DFrame;

class Serial {
private:
	DWORD btsIO;
	DCB lpTest;
	HANDLE hDevice;
	char recvText[LEN];
	SFrame sendF;
	DFrame sendD;
	DWORD error;
public:
	short id;
	float x, y;
	short angle;

	bool initSerial(char* com);
	int serialClose();

	void sendFrame(short id, float x, float y, float vx, float vy, short dA, float angV);
	void sendDebug(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3);
	void recvFrame();
};