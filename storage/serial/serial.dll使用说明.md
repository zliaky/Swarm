# serial.dll使用说明

## 简介
这是一个用C++写成的串口通信工具，源代码见[serial.h](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial.h)和[serial.cpp](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial.cpp)

## 使用说明
### 硬件
为保证串口连接成功，需要将搭载了XBEE的arduino连接在PC的COM5上

目前端口写死为了COM5，后续会改为通过参数决定端口号

为了数据类型和arduino一致，统一使用了short（2字节）、float（4字节）、char（1字节）。之后写参数也要注意这个问题

### 使用示例
```c++
// 姑且写成了c++的，c#写法应该差不多吧

Serial s;		       	// 定义一个Serial对象

if (s.initSerial()) {	// 初始化串口，这步没连串口的话会直接提醒port open failed并返回false
	s.serialLoop();		// 循环读串口内容，这个函数是无限循环所以要写多线程（此处省略）

	// 四个数据：id, x, y, angle。注意数据类型
	short id = *s.recvF.id;
	float x = *s.recvF.x;
	float y = *s.recvF.y;
	short angle = *s.recvF.angle;

	// 印象中C#好像不能用指针，如果这个方法不行的话我就在c++这边多写一步转换
	// 这边好像少写了一步校验……回头记得改
}
```

### Serial类说明
- Frame结构
   - `char *start, *start1`：包头验证位
   - `short *len`：数据长度，暂时没用上，先放着
   - `short *id`：机器人id
   - `float *x, *y`：机器人坐标
   - `short *angle`：机器人角度
   - `float *checkSum`：校验和
   - `char *frameEnd`：包尾验证位
- Serial类
   - private的就别管它了吧
   - `Frame sendF, recvF`：用于通信的两个Frame对象，目前只写完了接收（recvF）
   - `bool initSerial()`：初始化串口函数，初始化成功返回true，否则返回false
   - `void serialLoop()`：不断收发信息。因为是无限循环所以需要写多线程
   - `void sendFrame()`：发送信息的函数，没写完
   - `void recvFrame()`：接收信息的函数，突然发现还要再写一步校验和从指针到数据类型的转换