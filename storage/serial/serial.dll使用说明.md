# serial.dll使用说明

## 简介
这是一个用C++写成的串口通信工具，点击[这里](https://github.com/zliaky/Swarm/raw/master/storage/serial/Debug/serial.dll)下载

源代码见[serial.h](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial.h)和[serial.cpp](https://github.com/zliaky/Swarm/blob/master/storage/serial/serial/serial.cpp)

## 使用说明
### 硬件
为保证串口连接成功，需要将搭载了XBEE的arduino连接在PC的COM5上

目前端口写死为了COM5，后续会改为通过参数决定端口号

为了数据类型和arduino一致，统一使用了short（2字节）、float（4字节）、char（1字节）。之后写参数也要注意这个问题

### 使用示例
```c#
// unity代码使用示例，含多线程写法。不完整，不能直接复制粘贴
// 没测过，出问题再改吧
using System.Threading;

// 省略：Robo_frame定义代码和一大段dll引入代码
// 一共5个函数: initWrapper, closeWrapper, sendWrapper, recvWrapper, getInfor

private Thread dataRecvThread;
private Robo_frame robo_frame = new Robo_frame();
static int roboId;
static Vector2 roboPos;
static float roboAngle;

void Start()
{
    if (initWrapper()) {
        dataRecvThread = new Thread(new ThreadStart(DataRecv));
        dataRecvThread.Start();
    }
}

void Update()
{
    // 什么都不写
}

void OnApplicationQuit()
{
    closeWrapper();
}

void DataSend()
{
    // 因为这个功能还没实现所以先空着
}

void DataRecv()
{
    while (true)
    {
        recvWrapper();
        getInfor(ref robo_frame);
        roboId = robo_frame.id;
        roboPos = new Vector2(robo_frame.x, robo_frame.y);
        roboAngle = robo_frame.angle;
    }
}

// 后略：getPos函数

```

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
   - `void closeWraper()`：关闭串口
   - `void getInfor(frame_w &f)`：将serial类的内容转化为frame_w结构体的内容