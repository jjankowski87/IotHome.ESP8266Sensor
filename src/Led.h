#ifndef Led_h
#define Led_h

#include "Enums.h"
#include <Arduino.h>
#include <Ticker.h>

class Led
{
    public:
        Led(byte greenPin, byte redPin);
        ~Led();

        void blink(LedColor color, float seconds);
        void off(LedColor color);
    private:
        byte _greenPin;
        Ticker* _greenTicker;
        
        byte _redPin;
        Ticker* _redTicker;

        void toggle(byte pin);
};

#endif