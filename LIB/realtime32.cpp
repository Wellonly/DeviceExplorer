#include "realtime32.h"

Realtime32::Realtime32()
{

}

Realtime32::~Realtime32()
{

}

QString Realtime32::ToString(uint realtime)
{
    uint hours = realtime / RealTimeHour_realticks;
    uint minutes = (realtime - (hours * RealTimeHour_realticks)) / RealTimeMinute_realticks;
    uint seconds = (realtime - (hours * RealTimeHour_realticks) - (minutes * RealTimeMinute_realticks)) / RealTimeSecond_realticks;
    return QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds);
}


