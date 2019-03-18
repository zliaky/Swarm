#include "windows.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <conio.h>
#include <sstream>
using namespace std;

typedef struct {
	char *start;		//1
	short *len;			//2
	short *id;			//2
	float *x;			//4
	float *y;			//4
	short *angle;		//2
	float *checkSum;    //4
	char *frameEnd;     //1
}Frame;

/*
 * 发送字节格式：定位坐标Xt, 定位坐标Yt，//传感器状态
 * 接收字节格式：定位坐标Xc，定位坐标Yc，//传感器状态
 * 坐标格式：三位数字，单位mm，不足三位的部分补0
 */
Frame sendF;
Frame recvF;
char recvText[20];

int main() {

	//创建串口
	HANDLE hDevice = CreateFile(L"\\\\.\\COM5",  //注意COM1~COM10与COM10以上的窗口的命名规则
		GENERIC_READ | GENERIC_WRITE,   //使用读写方式
		0,
		0,
		OPEN_EXISTING,
		0,
		0);

	if (hDevice != INVALID_HANDLE_VALUE) {
		printf("Port opened!\n");
		DCB lpTest;
		GetCommState(hDevice, &lpTest);  //获取当前的参数设置
		lpTest.BaudRate = CBR_9600;       //设置波特率
		lpTest.ByteSize = 8;              //数据位数为8
		lpTest.Parity = NOPARITY;         //无校验
		lpTest.StopBits = ONESTOPBIT;   //1位停止位
		SetCommState(hDevice, &lpTest);  //设置通信参数
		DWORD btsIO;
		while (true) {
//			cout << "sending...\n";
//			WriteFile(hDevice, (char*)&sendF, sizeof(sendF), &btsIO, NULL);    //发送数据
//			cout << "write: " << sendF.x << ", " << sendF.y << endl;
//			ReadFile(hDevice, recvText, sizeof(recvText), &btsIO, NULL);
			ReadFile(hDevice, recvText, 1, &btsIO, NULL);
			if (recvText[0] != '~') {
				continue;
			}
			ReadFile(hDevice, recvText, 19, &btsIO, NULL);
/*			for (int i = 1; i < 19; i++) {
				cout << (short)recvText[i] << " ";
			}
			cout << endl;*/
			char *p = recvText;
			recvF.len = (short*)p;
			recvF.id = (short*)(p = p + sizeof(short));
			recvF.x = (float*)(p = p + sizeof(short));
			recvF.y = (float*)(p = p + sizeof(float));
			recvF.angle = (short*)(p = p + sizeof(float));
			recvF.checkSum = (float*)(p = p + sizeof(short));
			recvF.frameEnd = (char*)(p = p + sizeof(float));
//			cout << sizeof(char) << ", " << sizeof(short) << ", " << sizeof(float) << endl;
			if (*recvF.frameEnd == '!' && *recvF.checkSum == (*recvF.x + *recvF.y)) {
				cout << *recvF.id << ", " << *recvF.x << ", " << *recvF.y << ", " << *recvF.angle << endl;
			}
			Sleep(100);    //让程序暂停2s中，让二极管处在低电平维持一段时间
		}
		CloseHandle(hDevice);    //关闭串口 
	} else {
		printf("port open failed\n");
		system("pause");
	}

	return 0;
}