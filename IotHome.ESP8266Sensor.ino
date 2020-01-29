#include <FS.h> //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Ticker.h>
#include <ArduinoJson.h>

#include "src/iotc/common/string_buffer.h"
#include "src/iotc/iotc.h"

struct Config {
  char deviceName[20] = { 0 };
  char deviceKey[200] = { 0 };
};

const uint8_t BME_ADDRESS = 0x76;
const long SERIAL_BAUD = 9600;
const uint64_t SLEEP_TIME = 5 * 60e6; // 5 minutes
const unsigned long WIFI_TIMEOUT = 180; // seconds 

Adafruit_BME280 bme;
Ticker ticker;
Config config;
IOTContext context = NULL;

bool shouldSaveConfig = false;
bool isConnected = false;

// TODO: reset button which removes config file
void setup() { 
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(SERIAL_BAUD);

  if (!configureFileSystem() || !configureWiFi() || !configureBme() || !configureIoTHub()) {
    // TODO: what should we do when configuration fails?
    ESP.deepSleep(SLEEP_TIME);
    return;
  }

  ticker.attach(0.5, onTick);
  if (isConnected) {
    sendMessage("temperature", bme.readTemperature());
    sendMessage("humidity", bme.readHumidity());
  }
  
  ESP.deepSleep(SLEEP_TIME);
}

void loop() {
}

void onIotConnectionStatusChanged(IOTContext ctx, IOTCallbackInfo* callbackInfo) {
  isConnected = callbackInfo->statusCode == IOTC_CONNECTION_OK; 
  LOG_VERBOSE("Is connected ? %s (%d)", isConnected ? "YES" : "NO", callbackInfo->statusCode);
}

void onMessageSent(IOTContext ctx, IOTCallbackInfo* callbackInfo) {
  AzureIOT::StringBuffer buffer;
  if (callbackInfo->payloadLength > 0) {
    buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
  }

  LOG_VERBOSE("- Message to IotHub sent. Payload => %s\n", buffer.getLength() ? *buffer : "EMPTY");
}

void onWiFiConfigModeStarted(WiFiManager *myWiFiManager) {
  ticker.attach(0.1, onTick);
}

void onWiFiConfigSaved() {
  shouldSaveConfig = true;
}

void onTick() {
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

bool configureFileSystem() {
  if (!SPIFFS.begin()) {
    LOG_ERROR("Failed to mount File System");
    return false;  
  } 

  if (SPIFFS.exists("/config.json")) {
      File file = SPIFFS.open("/config.json", "r");
      if (file) {
        StaticJsonDocument<267> doc;

        DeserializationError error = deserializeJson(doc, file);
        if (!error)
        {
          strlcpy(config.deviceName, doc["deviceName"], sizeof(config.deviceName));
          strlcpy(config.deviceKey, doc["deviceKey"], sizeof(config.deviceKey));           
        } else {
          LOG_ERROR("Failed to read config file.");
        }
        
        file.close();
      }
  } else {
    LOG_VERBOSE("Config file doesn't exist");
  }

  return true;  
}

bool configureWiFi() {
  WiFiManager wifiManager;
  WiFiManagerParameter deviceNameParameter("device_name", "Device Name", config.deviceName, 20);
  WiFiManagerParameter deviceKeyParameter("device_key", "Device Key", config.deviceKey, 200);

  if (config.deviceKey[0] == 0 || config.deviceName[0] == 0) {
    LOG_VERBOSE("DeviceName or DeviceKey not set, configure via Wifi.");
        
    wifiManager.resetSettings();       
  }  

  wifiManager.addParameter(&deviceNameParameter);
  wifiManager.addParameter(&deviceKeyParameter);
  wifiManager.setAPCallback(onWiFiConfigModeStarted);
  wifiManager.setSaveConfigCallback(onWiFiConfigSaved);
  wifiManager.setTimeout(WIFI_TIMEOUT);
    
  if (!wifiManager.autoConnect()) {
    LOG_ERROR("Failed to connect to WiFi network.");
    return false;
  }

  if (shouldSaveConfig) {
    File file = SPIFFS.open("/config.json", "w");
    if (!file) {
      LOG_ERROR("Failed to open config file for writing.");
    }

    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<267> doc;
    
    strcpy(config.deviceName, deviceNameParameter.getValue());
    strcpy(config.deviceKey, deviceKeyParameter.getValue());
    doc["deviceName"] = config.deviceName;
    doc["deviceKey"] = config.deviceKey;

    if (serializeJson(doc, file) == 0) {
      LOG_ERROR("Failed to write config to file.");
    }

    file.close();
  }

  return true;
}

bool configureBme() {
  if (!bme.begin(BME_ADDRESS)) {
    LOG_ERROR("Could not find a valid BME280 sensor, check wiring!");
    return false;
  }

  return true;
}

bool configureIoTHub() {
  int errorCode = iotc_init_context(&context);
  if (errorCode != 0) {
    LOG_ERROR("Error initializing IOTC. Code %d", errorCode);
    return false;
  }

  iotc_set_logging(IOTC_LOGGING_API_ONLY);
  iotc_on(context, "ConnectionStatus", onIotConnectionStatusChanged, NULL);
  iotc_on(context, "MessageSent", onMessageSent, NULL);

  errorCode = iotc_connect(context, "", config.deviceKey, "", IOTC_CONNECT_CONNECTION_STRING);
  
  if (errorCode != 0) {
    LOG_ERROR("Error @ iotc_connect. Code %d", errorCode);
    return false;
  }

  return true;
}

void sendMessage(const char* sensor, float value) {
  int pos = 0;
  int errorCode = 0;

  char msg[80] = {0};
  int val_int = (int) value;
  int val_fra = (int) ((value - (float)val_int) * 100);
  pos = snprintf(msg, sizeof(msg) - 1, "{\"Sensor\": \"%s\", \"Name\": \"%s\", \"Value\": %d.%d}", sensor, config.deviceName, val_int, val_fra);

  iotc_send_telemetry(context, msg, pos);

  if (errorCode != 0) {
    LOG_ERROR("Sending message has failed with error code %d", errorCode);
  }
}
