#include "EnvironmentalSensor.h"
#include "iotc/iotc.h"

EnvironmentalSensor::EnvironmentalSensor(byte pin)
{
    _bme = new Adafruit_BME280();
    _isConnected = _bme->begin(pin);

    if (!_isConnected)
    {
        LOG_ERROR("Could not find a valid BME280 sensor, check wiring!");
    }
}

EnvironmentalSensor::~EnvironmentalSensor()
{
    delete _bme;
}

float EnvironmentalSensor::get(ReadingType readingType)
{
    switch (readingType)
    {
        case Temperature: return _bme->readTemperature();
        case Humidity: return _bme->readHumidity();
    }
}