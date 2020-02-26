#ifndef Connection_h
#define Connection_h

#include <WiFiManager.h>
#include "ConfigurationFile.h"

class Connection
{
    public:
        Connection(ConfigurationFile *configurationFile);
      
        bool initialize(); 
    private:
        ConfigurationFile *_configurationFile;
};

#endif
