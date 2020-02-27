#line 2 "IotHome.ESP8266Sensor.ino"  

// UNCOMMENT THIS LINE TO RUN UNIT TESTS
// #define TEST

#include <FS.h> //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "src/Constants.h"

#ifdef TEST
    #include <AUnit.h>
    #include "tests/ConfigurationTests.h"
    #include "tests/LedTests.h"

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
    #include "src/Application.h"

    Application* _application;

    void setup()
    {
        _application = new Application();
        Serial.begin(SERIAL_BAUD);
    }

    void loop()
    {
        _application->loop();
        // TODO: remove delay when released
        delay(50);
    }
#endif