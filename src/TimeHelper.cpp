#include "TimeHelper.h"

unsigned long TimeHelper::toMs(unsigned long seconds)
{
    return seconds * 1000;
}

unsigned long long TimeHelper::toUs(unsigned long seconds)
{
    return seconds * 1000000;
}