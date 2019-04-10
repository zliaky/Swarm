// serial_wrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "serial_wrapper.h"
#include "serial.h"

Serial s;

bool initWrapper(char* com) {
	//Serial s;
	if (s.initSerial(com)) {
		return true;
	}
	else
		return false;
}

void recvWrapper() {
	s.recvFrame();
}

void sendWrapper(short id, float x, float y, float vx, float vy, short dA, float angV) {
	s.sendFrame(id, x, y, vx,vy, dA, angV);
}

void sendDebugWrapper(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3) {
	s.sendDebug(id, dir1, pwm1, dir2, pwm2, dir3, pwm3);
}

int closeWrapper() {
	return s.serialClose();
}

void getInfor(frame_w &f) {
	f.id = s.id;
	f.x = s.x;
	f.y = s.y;
	f.angle = s.angle;
}
