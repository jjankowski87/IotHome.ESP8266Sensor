#include "Connection.h"
#include "Constants.h"
#include "Enums.h"
#include "TimeHelper.h"
#include "iotc/iotc.h"
#include <ESP8266HTTPClient.h>

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
    WiFiManagerParameter serviceUrlParameter("service_url", "Service Url (without / at the end)", config.serviceUrl, 200);

    wifiManager.addParameter(&deviceNameParameter);
    wifiManager.addParameter(&deviceKeyParameter);
    wifiManager.addParameter(&serviceUrlParameter);

    if (config.isEmpty() || resetWiFi)
    {
        _led->blink(LedColor::Red, BLINK_WIFI_SETUP);
        wifiManager.setTimeout(TimeHelper::toMs(WIFI_TIMEOUT));
        if (!wifiManager.startConfigPortal(AP_NAME, AP_PASS))
        {
            LOG_ERROR("Failed to connect to WiFi network.");
            _led->off(LedColor::Red);
            return false;
        }

        _led->off(LedColor::Red);
        strcpy(config.deviceName, deviceNameParameter.getValue());
        strcpy(config.deviceKey, deviceKeyParameter.getValue());
        strcpy(config.serviceUrl, serviceUrlParameter.getValue());

        _configurationFile->save(config.deviceName, config.deviceKey, config.serviceUrl);
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

long long Connection::getSleepTime(unsigned long startupTime)
{
    Config config = _configurationFile->load();

    char serviceUrl[sizeof(config.serviceUrl) + sizeof(NEXT_READING_PATH)];
    sprintf(serviceUrl, "%s%s", config.serviceUrl, NEXT_READING_PATH);

    HTTPClient http;
    bool result = http.begin(serviceUrl);
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK)
    {
        long sleepTime = http.getString().toInt();
        http.end();

        return TimeHelper::toUs(sleepTime);
    }

    LOG_ERROR("Cannot get sleep time from %s, response code: %d.", serviceUrl, httpCode);
    http.end();

    return getDefaultSleepTime(startupTime);
}

long long Connection::getDefaultSleepTime(unsigned long startupTime)
{
    unsigned long sleepTime = DEFAULT_SLEEP_TIME - (millis() - startupTime);

    if (sleepTime < 1)
    {
        return TimeHelper::toUs(1);
    }
    else if (sleepTime > DEFAULT_SLEEP_TIME)
    {
        return TimeHelper::toUs(DEFAULT_SLEEP_TIME);
    }

    return TimeHelper::toUs(sleepTime);
}
