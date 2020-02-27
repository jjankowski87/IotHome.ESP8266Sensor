#include "BmeSensor.h"
#include "iotc/iotc.h"

BmeSensor::BmeSensor(byte pin)
{
    _bme = new Adafruit_BME280();
    _isConnected = _bme->begin();

    if (!_isConnected)
    {
        LOG_ERROR("Could not find a valid BME280 sensor, check wiring!");
    }
}

BmeSensor::~BmeSensor()
{
    delete _bme;
}

float BmeSensor::get(ReadingType readingType)
{
    switch (readingType)
    {
        case Temperature: return _bme->readTemperature();
        case Humidity: return _bme->readHumidity();
    }
}