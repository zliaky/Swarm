#ifndef __OPTICALDATA__
#define __OPTICALDATA__

//导入arduino核心头文件
#include "Arduino.h"

typedef unsigned char uint8_t; // 8bit无符号类型
typedef unsigned long int uint32_t;  //32bit无符号类型

class OpticalData{
    private:
    uint8_t OID_SCK, OID_SDIO;
    double Ycoordinate;
    double Xcoordinate;
    double lastYcoordinate;
    double lastXcoordinate;
    int Angle;
    uint32_t RecvData[2];
    int state;

    protected:
    void WakeUpOID(void);
    void SendCmd(void);
    uint16_t RecvAck(void);
    int bin2angle(int* num);
    int bin2integer(int* num);
    double int2decimal(int* num);
    void ParseData(void);
    void RecvOptData(void);
    void Delay_10us(void);
    void Delay_78us(void);
    void Delay_30ms(void);
    void SendCmd(uint8_t Cmd);
    uint8_t Write_Register(uint16_t Data1,uint16_t Data2);
    
    public:
    OpticalData(uint8_t SCK_PIN, uint8_t SDIO_PIN);
    void begin(void);
    double getYcoordinate(void);
    double getXcoordinate(void);
    int getAngle(void);
    void OpticalDataInit(void);
    void CheckAndReadOpticalData(void);
    unsigned char TransmitCmd(unsigned char Cmd);
    void OIDPowerOffMode(void);
    void OIDWakeUpMode(void);
};

class KalmanInfo{
    private:
      double filterValue;  //k-1时刻的滤波值，即是k-1时刻的值
      double kalmanGain;   //   Kalamn增益
      double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
      double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
      double Q;   //预测过程噪声偏差的方差
      double R;   //测量噪声偏差，(系统搭建好以后，通过测量统计实验获得)
      double P;   //估计误差协方差
    
    public:
       void Init_KalmanInfo(double Q_init, double R_init);
       double KalmanFilter(double lastMeasurement);
};
#endif  // __OPTICALDATA__
