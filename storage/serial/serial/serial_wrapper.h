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

extern "C" bool _DLLExport initWrapper();
extern "C" void _DLLExport recvWrapper();
extern "C" void _DLLExport sendWrapper();
extern "C" int _DLLExport closeWrapper();
extern "C" void _DLLExport getInfor(frame_w &f);
