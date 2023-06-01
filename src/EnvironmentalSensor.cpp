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

    _bme->setSampling(Adafruit_BME280::MODE_FORCED,
        Adafruit_BME280::SAMPLING_X1, // temperature
        Adafruit_BME280::SAMPLING_NONE, // pressure
        Adafruit_BME280::SAMPLING_X1, // humidity
        Adafruit_BME280::FILTER_OFF);
}

EnvironmentalSensor::~EnvironmentalSensor()
{
    delete _bme;
}

float EnvironmentalSensor::get(ReadingType readingType)
{
    _bme->takeForcedMeasurement();

    switch (readingType)
    {
        case Temperature: return _bme->readTemperature();
        case Humidity: return _bme->readHumidity();
        default: return 0;
    }
}