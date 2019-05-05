#pragma once

#if defined (EXPORTBUILD)  
# define _DLLExport __declspec (dllexport)  
# else  
# define _DLLExport __declspec (dllimport)  
#endif  

typedef struct frame_w
{
	short id;
	float x;
	float y;
	short angle;
};

extern "C" bool _DLLExport initWrapper(char* com);
extern "C" void _DLLExport recvWrapper();
extern "C" void _DLLExport sendWrapper(short id, short mode, float x, float y);
extern "C" void _DLLExport sendDebugWrapper(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3);
extern "C" int _DLLExport closeWrapper();
extern "C" void _DLLExport getInfor(frame_w &f);
