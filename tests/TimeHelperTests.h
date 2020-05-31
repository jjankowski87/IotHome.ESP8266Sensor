#ifndef TimeHelperTests_h
#define TimeHelperTests_h

#include <AUnit.h>
#include "../src/TimeHelper.h"

test(ShouldConvertSecondsToMilliseconds)
{
    unsigned long seconds = 60 * 10;
    unsigned long ms = TimeHelper::toMs(seconds);

    unsigned long expected = 600000;
    assertEqual(ms, expected);
}

test(ShouldConvertSecondsToMicroseconds)
{
    unsigned long seconds = 60 * 10;
    unsigned long long us = TimeHelper::toUs(seconds);

    unsigned long long expected = 600000000;
    assertEqual(us, expected);
}

#endif
