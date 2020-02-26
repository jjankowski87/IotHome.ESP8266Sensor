#include "Connection.h"
#include "src/iotc/iotc.h"

Connection::Connection(Configuration *configuration)
{
  _configuration = configuration;
}

bool Connection::initialize()
{
  Config config = _configuration->load();
  
  WiFiManager wifiManager;
  WiFiManagerParameter deviceNameParameter("device_name", "Device Name", config.deviceName, 20);
  WiFiManagerParameter deviceKeyParameter("device_key", "Device Key", config.deviceKey, 200);

  if (config.isEmpty())
  {
    LOG_VERBOSE("DeviceName or DeviceKey not set, configure via Wifi.");
        
    wifiManager.resetSettings();       
  }  

  wifiManager.addParameter(&deviceNameParameter);
  wifiManager.addParameter(&deviceKeyParameter);
  //wifiManager.setAPCallback(onConfigMode);
  //wifiManager.setSaveConfigCallback(onConfigSaved);
  
  if (config.isEmpty())
  {
    wifiManager.setTimeout(180);  // TODO: const
    if (!wifiManager.startConfigPortal("OnDemandAP"))
    {
      LOG_ERROR("Failed to connect to WiFi network.");
      return false;
    }

    // TODO: reset? save config
    return false;
  }
  else
  {
    wifiManager.setTimeout(0);
    if (!wifiManager.autoConnect("AutoConnectAP")) // TODO: network name and password
    {
      LOG_ERROR("Failed to connect to WiFi network.");
      return false;
    }
  } 

  if (_shouldSaveConfig)
  {
    LOG_VERBOSE("Saving config");
    _configuration->save(config.deviceName, config.deviceKey);
  }

  return true;
}

void Connection::onConfigMode(WiFiManager *myWiFiManager)
{
  LOG_VERBOSE("Config mode entered");
}

void Connection::onConfigSaved()
{
  _shouldSaveConfig = true;
}
