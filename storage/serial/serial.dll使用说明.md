# serial.dll使用说明

## 简介
这是一个用C++写成的串口通信工具，点击[这里](https://github.com/zliaky/Swarm/raw/master/storage/serial/x64/Debug/serial.dll)下载

源代码见[serial.h](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial.h)、[serial.cpp](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial.cpp)、[serial_wrapper.h](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial_wrapper.h)、[serial_wrapper.cpp](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial_wrapper.cpp)

## 使用说明
### 硬件
为保证串口连接成功，需要将搭载了XBEE的arduino连接在PC的COM5上

目前端口写死为了COM5，后续会改为通过参数决定端口号

为了数据类型和arduino一致，统一使用了short（2字节）、float（4字节）、char（1字节）。之后写参数也要注意这个问题

### Serial类说明
- 常数
   - `LEN`：表示结构体总字节长度（目前是21）
- Frame结构
   - `char start, start1`：包头验证位
   - `short len`：数据长度，暂时没用上，先放着
   - `short id`：机器人id
   - `float x, y`：机器人坐标
   - `short angle`：机器人角度
   - `float checkSum`：校验和
   - `char frameEnd`：包尾验证位
- Serial类
   - private
   - `DWORD btsIO; DCB lpTest; HANDLE hDevice;`：串口通信用到的变量
   - `char sendText[LEN], recvText[LEN]`：在收发时实际用来传输数据的字符串
   - `Frame sendF`：用于发送信息的结构体，暂时没用到
   - public
   - `short id`：机器人id
   - `float x, y`：机器人坐标
   - `short angle`：机器人角度
   - `bool initSerial()`：初始化串口函数，初始化成功返回true，否则返回false
   - `void sendFrame()`：发送信息函数，没写完
   - `void recvFrame()`：接收信息函数，内置数据解码，调用一次收一条数据
   - `void serialClose()`：关闭串口
- dll函数
   - `frame_w`结构体：用于数据传输的一个结构体，包含机器人id，坐标和角度
   - `bool initWrapper()`：初始化串口函数，初始化成功返回true，否则返回false
   - `void sendWrapper()`：发送信息函数，没写完
   - `void recvWrapper()`：接收信息函数，内置数据解码，调用一次收一条数据
   - `int closeWraper()`：关闭串口，返回0表示没问题。返回其他值可以查询`DWORD GetError()`函数的错误信息
   - `void getInfor(frame_w &f)`：将serial类的内容转化为frame_w结构体的内容