#ifndef Application_h
#define Application_h

#include "EnvironmentalSensor.h"
#include "ConfigurationFile.h"
#include "Connection.h"
#include "Enums.h"
#include "IotHubClient.h"
#include "Keyboard.h"
#include "Led.h"

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

        EnvironmentalSensor* _sensor;
        ConfigurationFile* _configurationFile;
        Connection* _wifi;
        Keyboard* _keyboard;
        Led* _led;
        IotHubClient* _iotHubClient;

        AppState nextState();

        AppState setup();
        AppState wait();
        AppState initialize();
        AppState send(); 
        AppState error();
        AppState finished();

        inline bool timePassed(unsigned long time) { return millis() - _startupTime >= time; };
};

#endif