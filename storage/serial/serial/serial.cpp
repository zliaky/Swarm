﻿#include "stdafx.h"
#include "serial.h"

/*
 * 初始化串口
 * 返回值：true：打开成功；false：打开失败
 */
bool Serial::initSerial() {
	//创建串口
	hDevice = CreateFile(L"\\\\.\\COM5",	//注意COM1~COM10与COM10以上的窗口的命名规则
		GENERIC_READ | GENERIC_WRITE,		//使用读写方式
		0,
		0,
		OPEN_EXISTING,
		0,
		0);

	if (hDevice != INVALID_HANDLE_VALUE) {
		printf("Port opened!\n");
		GetCommState(hDevice, &lpTest);		//获取当前的参数设置
		lpTest.BaudRate = CBR_9600;			//设置波特率
		lpTest.ByteSize = 8;				//数据位数为8
		lpTest.Parity = NOPARITY;			//无校验
		lpTest.StopBits = ONESTOPBIT;		//1位停止位
		SetCommState(hDevice, &lpTest);		//设置通信参数
		return true;
	} else {
		printf("port open failed\n");
		return false;
	}
}

/* 
 * 串口循环收发信息
 */
void Serial::serialLoop() {
	while (true) {
		recvFrame();
		Sleep(100);	//让程序暂停0.1s中，让二极管处在低电平维持一段时间
	}
	CloseHandle(hDevice);	//关闭串口 
}

/*
 *发送数据 
 */
void Serial::sendFrame() {
	WriteFile(hDevice, (char*)&sendF, sizeof(sendF), &btsIO, NULL);	//发送数据
}

/*
 * 接收数据
 */
void Serial::recvFrame() {
	ReadFile(hDevice, recvText, 2, &btsIO, NULL);
	if (recvText[0] != '~' || recvText[1] != '~') {	//前两位验证位
		return;
	}
	ReadFile(hDevice, recvText, LEN-2, &btsIO, NULL);
	char *p = recvText;

	short _len = *((short*)p);
	short _id = *((short*)(p = p + sizeof(short)));
	float _x = *((float*)(p = p + sizeof(short)));
	float _y = *((float*)(p = p + sizeof(float)));
	short _angle = *((short*)(p = p + sizeof(float)));
	float _checkSum = *((float*)(p = p + sizeof(short)));
	char _frameEnd = *((char*)(p = p + sizeof(float)));
	if (_frameEnd == '!' && _checkSum == (_x + _y)) {
		id = _id;
		x = _x;
		y = _y;
		angle = _angle;
//		cout << id << ", " << x << ", " << y << ", " << angle << endl;
	}
}
