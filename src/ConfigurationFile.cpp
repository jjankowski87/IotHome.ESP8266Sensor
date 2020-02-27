#include <FS.h>
#include "ConfigurationFile.h"
#include "iotc/iotc.h"
#include <ArduinoJson.h>

const char * const ConfigurationFile::ConfigFileName = "/config.json";

ConfigurationFile::ConfigurationFile()
{
    _isMounted = SPIFFS.begin();

    if (!_isMounted)
    {
        LOG_ERROR("Failed to mount File System.");
    }
}

Config ConfigurationFile::load()
{ 
    Config config;
    if (!SPIFFS.exists(ConfigFileName))
    {
        LOG_VERBOSE("Config file doesn't exist.");
        return config;
    }

    File file = SPIFFS.open(ConfigFileName, "r");
    if (!file)
    {
        return config;
    }

    StaticJsonDocument<267> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (!error)
    {
        strlcpy(config.deviceName, doc["deviceName"], sizeof(config.deviceName));
        strlcpy(config.deviceKey, doc["deviceKey"], sizeof(config.deviceKey));
    }
    else
    {
        LOG_ERROR("Failed to read config file.");
    }

    file.close();
    return config;
}

void ConfigurationFile::save(char deviceName[20], char deviceKey[200])
{
    File file = SPIFFS.open(ConfigFileName, "w");
    if (!file)
    {
        LOG_ERROR("Failed to open config file for writing.");
        return;
    }

    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<267> doc;

    doc["deviceName"] = deviceName;
    doc["deviceKey"] = deviceKey;

    if (serializeJson(doc, file) == 0)
    {
        LOG_ERROR("Failed to write config to file.");
    }

    file.close();
}

void ConfigurationFile::remove()
{
    if (SPIFFS.exists(ConfigFileName))
    {
        SPIFFS.remove(ConfigFileName);
    }
}
