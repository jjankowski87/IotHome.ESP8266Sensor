#line 2 "IotHome.ESP8266Sensor.ino"  

// UNCOMMENT THIS LINE TO RUN UNIT TESTS
// #define TEST

#ifdef TEST
    #include <AUnit.h>
    #include "tests/ConfigurationTests.h"
    #include "tests/LedTests.h"
    #include "tests/TimeHelperTests.h"
    #include "src/Constants.h"

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
    #include "src/Constants.h"

    Application* _application;

    void turnOffBuiltInLed(){
        pinMode(BUILTIN_LED, OUTPUT);
        digitalWrite(BUILTIN_LED, HIGH);
    }

    void setup()
    {
        Serial.begin(SERIAL_BAUD);
        turnOffBuiltInLed();

        _application = new Application();
    }

    void loop()
    {
        _application->loop();
    }
#endif