#ifndef REALTIME32_H
#define REALTIME32_H

#include <QString>

class Realtime32
{
public:
    Realtime32();
    ~Realtime32();

    static QString ToString(uint realtime);


    static const int RealTimeMillisecond_realticks_rounded = 33; //32,768
    static const int RealTimeSecond_realticks = 32768;
    static const int RealTimeMinute_realticks = 1966080;
    static const int RealTimeHour_realticks = 117964800;

};

#endif // REALTIME32_H
