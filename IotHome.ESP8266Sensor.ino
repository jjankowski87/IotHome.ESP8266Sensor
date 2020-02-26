#line 2 "IotHome.ESP8266Sensor.ino"  

// UNCOMMENT THIS LINE TO RUN UNIT TESTS
#define TEST

#include <FS.h> //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "Constants.h"

#ifdef TEST
  #include <AUnit.h>
  #include "ConfigurationTests.h"

  void setup()
  {
    delay(1000);
    Serial.begin(SERIAL_BAUD);
  }
  
  void loop()
  {
    aunit::TestRunner::run();
  }
#else
  #include "src/iotc/common/string_buffer.h"
  #include "src/iotc/iotc.h"
  #include "Led.h"
  #include "Configuration.h"
  #include "Connection.h"
  #include "Enums.h"

  Adafruit_BME280 bme;
  IOTContext context = NULL;
  
  Led *led;
  Configuration *configuration;
  Connection *connection;
  
  AppState appState = AppState::Waiting;
  bool isConnected = false;
  unsigned long startupTime;
  
  inline bool isButtonPressed(){
    return digitalRead(RESET_BUTTON_PIN) == HIGH;
  }
  
  inline bool timePassed(unsigned long since, unsigned long time) {
    return millis() - since >= time;
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
  
    errorCode = iotc_connect(context, "", "TODO: config.deviceKey", "", IOTC_CONNECT_CONNECTION_STRING);
    
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
    pos = snprintf(msg, sizeof(msg) - 1, "{\"Sensor\": \"%s\", \"Name\": \"%s\", \"Value\": %d.%d}", sensor, "config.deviceName", val_int, val_fra); // TODO: device Name
  
    iotc_send_telemetry(context, msg, pos);
  
    if (errorCode != 0) {
      LOG_ERROR("Sending message has failed with error code %d", errorCode);
    }
  }
  
  void wait()
  {
    if (isButtonPressed())
    {
      configuration->remove();    
      
      led->off(LedColor::Green);
      appState = AppState::Initializing;
    }
    else if (timePassed(startupTime, INIT_DELAY))
    {
      led->off(LedColor::Green);
      appState = AppState::Initializing;
    }
  }
  
  void initialize(){
    led->blink(LedColor::Red, 0.5);
  
    configuration->load();
    delay(5000);
  
    led->off(LedColor::Red);
  
    appState = AppState::Sending;
  }
  
  void send()
  {  
  }

  void setup()
  {
    Serial.begin(SERIAL_BAUD);
    startupTime = millis();
    pinMode(RESET_BUTTON_PIN, INPUT);
    
    led = new Led(GREEN_LED_PIN, RED_LED_PIN);
    configuration = new Configuration();
    connection = new Connection(configuration);
      
    led->blink(LedColor::Green, 0.5);
    configuration->initialize();  
  //  if (!configureFileSystem() || !configureWiFi() || !configureBme() || !configureIoTHub()) {
  //    // TODO: what should we do when configuration fails?
  //    ESP.deepSleep(SLEEP_TIME);
  //    return;
  //  }
  //
  //  ticker.attach(0.5, onTick);
  //  if (isConnected) {
  //    sendMessage("temperature", bme.readTemperature());
  //    sendMessage("humidity", bme.readHumidity());
  //  }
  //  
  //  ESP.deepSleep(SLEEP_TIME);
  }
  
  void loop()
  {
    switch (appState) {
      case Waiting: wait(); break;
      case Initializing: initialize(); break;
      case Sending: send(); break;
    }
  }
#endif
