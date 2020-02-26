#ifndef Connection_h
#define Connection_h

#include <WiFiManager.h>   
#include "Configuration.h"

class Connection 
{
public:
  Connection(Configuration *configuration);
  
  bool initialize(); 
private:
  bool _shouldSaveConfig;
  Configuration *_configuration;
  
  void onConfigSaved();
  void onConfigMode(WiFiManager *myWiFiManager);
};

#endif
