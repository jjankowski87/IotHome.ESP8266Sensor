#include "Connection.h"
#include "Constants.h"
#include "Enums.h"
#include "iotc/iotc.h"

Connection::Connection(ConfigurationFile* configurationFile, Led* led)
{
    _configurationFile = configurationFile;
    _led = led;
}

bool Connection::connect(bool resetWiFi)
{
    WiFiManager wifiManager;
    if (resetWiFi)
    {
        wifiManager.resetSettings();
    }

    Config config = _configurationFile->load();
    WiFiManagerParameter deviceNameParameter("device_name", "Device Name", config.deviceName, 20);
    WiFiManagerParameter deviceKeyParameter("device_key", "Device Key", config.deviceKey, 200);

    wifiManager.addParameter(&deviceNameParameter);
    wifiManager.addParameter(&deviceKeyParameter);

    if (config.isEmpty() || resetWiFi)
    {
        _led->blink(LedColor::Red, BLINK_WIFI_SETUP);
        wifiManager.setTimeout(WIFI_TIMEOUT);
        if (!wifiManager.startConfigPortal(AP_NAME, AP_PASS))
        {
            LOG_ERROR("Failed to connect to WiFi network.");
            _led->off(LedColor::Red);
            return false;
        }

        _led->off(LedColor::Red);
        strcpy(config.deviceName, deviceNameParameter.getValue());
        strcpy(config.deviceKey, deviceKeyParameter.getValue());

        _configurationFile->save(config.deviceName, config.deviceKey);
        return true;
    }

    wifiManager.setTimeout(1);
    if (!wifiManager.autoConnect(AP_NAME, AP_PASS))
    {
        LOG_ERROR("Failed to connect to WiFi network.");
        return false;
    }

    return true;
}