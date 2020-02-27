#ifndef Connection_h
#define Connection_h

#include <WiFiManager.h>
#include "ConfigurationFile.h"
#include "Led.h"

class Connection
{
    public:
        Connection(ConfigurationFile* configurationFile, Led* led);
      
        bool connect(bool resetWiFi); 
    private:
        ConfigurationFile* _configurationFile;
        Led* _led;
};

#endif
