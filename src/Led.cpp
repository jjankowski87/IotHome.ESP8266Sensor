#include "Led.h"

Led::Led(byte greenPin, byte redPin)
{
    _greenPin = greenPin;
    _greenTicker = new Ticker();
    pinMode(_greenPin, OUTPUT); 
    digitalWrite(_greenPin, HIGH);

    _redPin = redPin;
    _redTicker = new Ticker();
    pinMode(_redPin, OUTPUT);
    digitalWrite(_redPin, HIGH);
}

Led::~Led()
{
    delete _greenTicker;
    delete _redTicker;
}

void Led::blink(LedColor color, float seconds)
{
    switch (color)
    {
        case Green:
            _greenTicker->attach(seconds, [&]() {  toggle(_greenPin); });
            break;
        case Red:
            _redTicker->attach(seconds, [&]() {  toggle(_redPin); });
            break;
    }
}

void Led::on(LedColor color)
{
    switch (color)
    {
        case Green:
            _greenTicker->detach();
            digitalWrite(_greenPin, LOW);
            break;
        case Red:
            _redTicker->detach();
            digitalWrite(_redPin, LOW);
            break;
    }
}

void Led::off(LedColor color)
{
    switch (color)
    {
        case Green:
            _greenTicker->detach();
            digitalWrite(_greenPin, HIGH);
            break;
        case Red:
            _redTicker->detach();
            digitalWrite(_redPin, HIGH);
            break;
    }
}

void Led::toggle(byte pin) 
{
    int state = digitalRead(pin);
    digitalWrite(pin, !state);
}
