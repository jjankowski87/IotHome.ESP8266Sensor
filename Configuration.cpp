#include <FS.h>
#include "Configuration.h"
#include "src/iotc/iotc.h"
#include <ArduinoJson.h>

const char * const Configuration::FileName = "/config.json";

bool Configuration::initialize()
{
  if (!SPIFFS.begin())
  {
    LOG_ERROR("Failed to mount File System.");
    return false;  
  }

  return true;
}

Config Configuration::load()
{
  if (!_config.isEmpty())
  {
    return _config;
  }
  
  if (SPIFFS.exists(Configuration::FileName))
  {
      File file = SPIFFS.open(Configuration::FileName, "r");
      if (file)
      {
        StaticJsonDocument<267> doc;

        DeserializationError error = deserializeJson(doc, file);
        if (!error)
        {
          strlcpy(_config.deviceName, doc["deviceName"], sizeof(_config.deviceName));
          strlcpy(_config.deviceKey, doc["deviceKey"], sizeof(_config.deviceKey));
        }
        else
        {
          LOG_ERROR("Failed to read config file.");
        }
        
        file.close();
      }
  } else {
    LOG_VERBOSE("Config file doesn't exist.");
  }

  return _config;
}

void Configuration::save(char deviceName[20], char deviceKey[200])
{
  File file = SPIFFS.open(FileName, "w");
  if (!file)
  {
    LOG_ERROR("Failed to open config file for writing.");
  }

  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<267> doc;
  
  doc["deviceName"] = deviceName;
  doc["deviceKey"] = deviceKey;

  if (serializeJson(doc, file) == 0)
  {
    LOG_ERROR("Failed to write config to file.");
  }
  else
  {
    // TODO
    // _config.deviceName = &deviceName;
    // _config.deviceKey = &deviceKey;
  }

  file.close();
}

void Configuration::remove() 
{
  if (SPIFFS.exists(Configuration::FileName))
  {
    SPIFFS.remove(Configuration::FileName);
    //_config = Config;
  } 
  else
  {
    LOG_VERBOSE("Config file doesn't exist, could not remove.");
  }  
}
