#ifndef EnvironmentalSensor_h
#define EnvironmentalSensor_h

#include "Enums.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class EnvironmentalSensor
{
    public:
        EnvironmentalSensor(byte pin);
        ~EnvironmentalSensor();

        float get(ReadingType readingType);
        inline bool isConnected() { return _isConnected; };
    private:
        bool _isConnected;
        Adafruit_BME280* _bme;
};

#endif