#include "Connection.h"
#include "Constants.h"
#include "iotc/iotc.h"

Connection::Connection(ConfigurationFile *configurationFile)
{
    _configurationFile = configurationFile;
}

bool Connection::initialize()
{
    WiFiManager wifiManager;
    Config config = _configurationFile->load();
    if (config.isEmpty())
    {
      wifiManager.resetSettings();
    }

    WiFiManagerParameter deviceNameParameter("device_name", "Device Name", config.deviceName, 20);
    WiFiManagerParameter deviceKeyParameter("device_key", "Device Key", config.deviceKey, 200);

    wifiManager.addParameter(&deviceNameParameter);
    wifiManager.addParameter(&deviceKeyParameter);

    if (config.isEmpty())
    {
        wifiManager.setTimeout(WIFI_TIMEOUT);
        if (!wifiManager.startConfigPortal(AP_NAME, AP_PASS))
        {
            LOG_ERROR("Failed to connect to WiFi network.");
            return false;
        }

      // TODO: reset?
      LOG_VERBOSE("Config updated");
      _configurationFile->save(config.deviceName, config.deviceKey);
      return true;
    }

    wifiManager.setTimeout(0);
    if (!wifiManager.autoConnect(AP_NAME, AP_PASS))
    {
        LOG_ERROR("Failed to connect to WiFi network.");
        return false;
    }

    return true;
}