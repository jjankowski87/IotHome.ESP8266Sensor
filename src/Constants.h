#ifndef Constants_h
#define Constants_h

const uint8_t GREEN_LED_PIN = 0x0E;
const uint8_t RED_LED_PIN = 0x0C;
const uint8_t RESET_BUTTON_PIN = 0x0F;
const uint8_t BME_ADDRESS = 0x76;

const long SERIAL_BAUD = 115200;

// all times are in seconds
const unsigned long DEFAULT_SLEEP_TIME = 10 * 60;
const unsigned long WIFI_TIMEOUT = 180;
const unsigned long INIT_DELAY = 60;
const unsigned long RESTART_SLEEP_TIME = 30;

const float BLINK_WIFI_SETUP = 2;
const float BLINK_SEND = 0.2;
const float BLINK_ERROR = 0.2;

const char AP_NAME[] = "IoTSensor";
const char AP_PASS[] = "password";

const char NEXT_READING_PATH[] = "/api/time/next-reading/";

#endif
