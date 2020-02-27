#ifndef BmeSensor_h
#define BmeSensor_h

#include "Enums.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class BmeSensor
{
    public:
        BmeSensor(byte pin);
        ~BmeSensor();

        float get(ReadingType readingType);
        inline bool isConnected() { return _isConnected; };
    private:
        bool _isConnected;
        Adafruit_BME280* _bme;
};

#endif