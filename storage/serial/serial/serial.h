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
/*
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
*/
typedef struct {
	char start;		//��֤λ1
	char start1;		//��֤λ2
	short len;			//���ݳ��ȣ���ʱû����
	short id;			//������id
	short mode;
	float x;			//Ŀ������
	float y;
	float checkSum;    //У���
	char frameEnd;     //��β��֤λ
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