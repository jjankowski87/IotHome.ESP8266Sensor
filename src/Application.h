#ifndef Application_h
#define Application_h

#include "EnvironmentalSensor.h"
#include "ConfigurationFile.h"
#include "Connection.h"
#include "Enums.h"
#include "IotHubClient.h"
#include "Keyboard.h"
#include "Led.h"
#include "TimeHelper.h"

struct State
{
    bool resetWiFi = false;
};

class Application
{
    public:
        Application();
        ~Application();

        void loop();
    private:
        AppState _appState;
        State _state;
        unsigned long _startupTime;

        float _temperature;
        float _humidity;

        EnvironmentalSensor* _sensor;
        ConfigurationFile* _configurationFile;
        Connection* _wifi;
        Keyboard* _keyboard;
        Led* _led;
        IotHubClient* _iotHubClient;

        AppState nextState();

        AppState setup();
        AppState readSensor();
        AppState wait();
        AppState initialize();
        AppState send(); 
        bool sendTemperature(Config config);
        bool sendHumidity(Config config);
        AppState error();
        AppState finished();

        inline bool timePassed(unsigned long seconds) { return millis() - _startupTime >= TimeHelper::toMs(seconds); };
};

#endif