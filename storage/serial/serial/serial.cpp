#include "stdafx.h"
#include "serial.h"

/*
 * 初始化串口
 * 返回值：true：打开成功；false：打开失败
 */
bool Serial::initSerial(char* com) {
	//创建串口
	hDevice = CreateFile(LPCTSTR(com),	//注意COM1~COM10与COM10以上的窗口的命名规则
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
		FlushFileBuffers(hDevice);
		return true;
	} else {
		printf("port open failed\n");
		return false;
	}
}

/* 
 * 关闭串口
 */
int Serial::serialClose() {
	FlushFileBuffers(hDevice);
	if (CloseHandle(hDevice)) {
		return 0;
	} else {
		return GetLastError();
	}
}

/*
 *发送数据 
 */
/*
void Serial::sendFrame(short id, float x, float y, float vx, float vy, short dA, float angV) {
	SFrame sendF;
	sendF.start = '~';
	sendF.start1 = '~';
	sendF.len = 48;
	sendF.id = id;
	sendF.x = x;
	sendF.y = y;
	sendF.vx = vx;
	sendF.vy = vy;
	sendF.dA = dA;
	sendF.angV = angV;
	sendF.checkSum = sendF.x + sendF.y;
	sendF.frameEnd = '!';
	WriteFile(hDevice, (char*)&sendF, sizeof(sendF), &btsIO, NULL);	//发送数据
}
*/
void Serial::sendFrame(short id, short mode, float x, float y)
{
	SFrame sendF;
	sendF.start = '~';
	sendF.start1 = '~';
	sendF.len = 48;
	sendF.id = id;
	sendF.mode = mode;
	sendF.x = x;
	sendF.y = y;
	sendF.checkSum = sendF.x + sendF.y;
	sendF.frameEnd = '!';
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


void Serial::sendDebug(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3)
{
	DFrame sendD;
	sendD.start = 'D';
	sendD.start1 = 'e';
	sendD.id = (short)id;
	sendD.dir0 = (short)dir1;
	sendD.dir1 = (short)dir2;
	sendD.dir2 = (short)dir3;
	sendD.pwm0 = (short)pwm1;
	sendD.pwm1 = (short)pwm2;
	sendD.pwm2 = (short)pwm3;
	sendD.checkSum = sendD.dir0 + sendD.pwm0;
	sendD.frameEnd = '!';
	WriteFile(hDevice, (char*)&sendD, sizeof(sendD), &btsIO, NULL);	//发送数据

}