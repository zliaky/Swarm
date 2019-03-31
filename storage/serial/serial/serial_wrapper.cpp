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

void sendWrapper(short id, float x, float y, float v) {
	s.sendFrame(id, x, y, v);
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
