#ifndef Constants_h
#define Constants_h

const uint8_t GREEN_LED_PIN = 0x0E;
const uint8_t RED_LED_PIN = 0x0C;
const uint8_t RESET_BUTTON_PIN = 0x0F;
const uint8_t BME_ADDRESS = 0x76;

const long SERIAL_BAUD = 115200;
const uint64_t SLEEP_TIME = 5 * 60e6; // 5 minutes
const unsigned long WIFI_TIMEOUT = 180; // seconds
const unsigned long INIT_DELAY = 3000; // ms

const float BLINK_WIFI_SETUP = 2;
const float BLINK_SEND = 0.2;
const float BLINK_ERROR = 0.2;

const char AP_NAME[] = "IoTSensor";
const char AP_PASS[] = "password";

#endif
