#ifndef Connection_h
#define Connection_h

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "ConfigurationFile.h"
#include "Led.h"

class Connection
{
    public:
        Connection(ConfigurationFile* configurationFile, Led* led);

        bool connect(bool resetWiFi);
        long long getSleepTime(unsigned long startupTime);
    private:
        ConfigurationFile* _configurationFile;
        Led* _led;
};

#endif
