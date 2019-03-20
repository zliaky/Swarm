// serial_wrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "serial_wrapper.h"
#include "serial.h"

Serial s;

bool initWrapper() {
	//Serial s;
	if (s.initSerial()) {
		return true;
	}
	else
		return false;
}

void loopWrapper() {
	s.serialLoop();
}

void getInfor(frame_w &f) {
	f.id = s.id;
	f.x = s.x;
	f.y = s.y;
	f.angle = s.angle;
}


