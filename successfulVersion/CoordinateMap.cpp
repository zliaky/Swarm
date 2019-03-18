#include "CoordinateMap.h"

double Xcode2Xactual(double Xcode){
    double Xactual;
    Xactual=Xratio * (Xcode-Xdelta);
    return Xactual;
}

double Ycode2Yactual(double Ycode){
    double Yactual;
    Yactual=Yratio * (Ycode-Ydelta);
    return Yactual;
}

int Acode2Angle(int Acode){
    int Angle;
    Angle= Acode-AngleDelta;
    return Angle;
}
