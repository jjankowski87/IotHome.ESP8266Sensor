#ifndef TimeHelper_h
#define TimeHelper_h

class TimeHelper
{
    public:
        static unsigned long toMs(unsigned long seconds);
        static unsigned long long toUs(unsigned long seconds);
    private:
        TimeHelper() {}
};

#endif