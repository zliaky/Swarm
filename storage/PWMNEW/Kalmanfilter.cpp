#include "OpticalData.h"

void KalmanInfo::Init_KalmanInfo(double Q_init, double R_init){
    A=1;//标准卡尔曼
    H=1;
    P=3;//后验状态估计值误差的方差的初始值（不要为0问题不大）
    Q=Q_init;//预测（过程）噪声方差 影响收敛速率，可以根据实际需求给出
    R=R_init;//测量（观测）噪声方差 可以通过实验手段获得
    filterValue=0;//测量的初始值
}

double KalmanInfo::KalmanFilter(double lastMeasurement)
{
    //预测下一时刻的值
    double predictValue = A*filterValue;//x的先验估计由上一个时间点的后验估计值和输入信息给出，此处需要根据基站高度做一个修改
    //求协方差
     P=A*A*P+Q; //计算先验均方差p(n|n-1)
    double preValue = filterValue;//记录上次坐标实际坐标值
    //计算卡尔曼增益
    kalmanGain = P*H/(P*H*H+R); //Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R)
    //修正结果，计算滤波值
    filterValue = predictValue + (lastMeasurement - predictValue)*kalmanGain; //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
    //更新后验估计
    P = (1-kalmanGain*H)*P;//计算后验均方差  P[n|n]=(1-K[n]*H)*P[n|n-1]
    return filterValue;
}
