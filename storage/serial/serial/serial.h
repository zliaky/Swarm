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
	char *start;		//��֤λ1
	char *start1;		//��֤λ2
	short *len;			//���ݳ��ȣ���ʱû����
	short *id;			//������id
	float *x;			//����������
	float *y;			//����������
	short *angle;		//�����˽Ƕ�
	float *checkSum;    //У���
	char *frameEnd;     //��β��֤λ
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