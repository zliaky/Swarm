#include "OpticalData.h"

void KalmanInfo::Init_KalmanInfo(double Q_init, double R_init,double N_limitInit, double N_disinit){
    A=1;//标准卡尔曼
    H=1;
    P=3;//后验状态估计值误差的方差的初始值（不要为0问题不大）
    Q=Q_init;//预测（过程）噪声方差 影响收敛速率，可以根据实际需求给出
    R=R_init;//测量（观测）噪声方差 可以通过实验手段获得
    filterValue=0;//测量的初始值
    lastData=0;
    //N_limit = N_limitInit;
    N_ldtm = N_limitInit;
    N_dis = N_disinit;
    count=0;
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
/*
1、限幅滤波法（又称程序判断滤波法）
A、方法：
根据经验判断，确定两次采样允许的最大偏差值（设为A）
每次检测到新值时判断：
如果本次值与上次值之差<=A,则本次值有效
如果本次值与上次值之差>A,则本次值无效,放弃本次值,用上次值代替本次值
B、优点：
能有效克服因偶然因素引起的脉冲干扰
C、缺点
无法抑制那种周期性的干扰
平滑度差
*/

double  KalmanInfo::filter_limit(double data){
    double new_value;
    new_value = data;
   // cout <<"get_data产生的数据:"<< new_value << endl;
    if ((new_value - lastData > N_limit)||(lastData - new_value > N_limit))
        return lastData;
        
    lastData=new_value;
    return new_value;
}

/*6.限幅平均滤波法函数体*/
/*
6、限幅平均滤波法
A、方法：
相当于“限幅滤波法”+“递推平均滤波法”
每次采样到的新数据先进行限幅处理，
再送入    队列进行递推平均滤波处理
B、优点：
融合了两种滤波法的优点
对于偶然出现的脉冲性干扰，可消除由于脉冲干扰所引起的采样值偏差
C、缺点：
比较浪费RAM
*/
double  KalmanInfo::filter_limdtm(double data){
    double new_value;
    int i;
    double sum = 0;

    count=count+1;
    new_value = data;
    if ((new_value - lastData>N_ldtm) || (lastData - new_value>N_ldtm))
    {
        value_ldtm_buf[N_lddtm] = lastData;
    }
    else
    {
        value_ldtm_buf[N_lddtm] = new_value;
    }
    for ( i = 0; i < N_lddtm; i++)
    {
        value_ldtm_buf[i] = value_ldtm_buf[i + 1];//先进先出所有数组元素向左移，首位舍弃。
        sum += value_ldtm_buf[i];
    }
    lastData=(double)(sum / N_lddtm);
    return (double)(sum / N_lddtm);
}

void KalmanInfo::filter_update(double data){
     // lastData = data;
      N_ldtm=(data/N_dis+1)*N_dis;      
      }                                                       
