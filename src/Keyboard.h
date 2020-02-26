#ifndef Keyboard_h
#define Keyboard_h

#include "Enums.h"

class Keyboard
{
    public:
        Keyboard(byte resetPin) { _resetPin = resetPin; };

        inline bool isButtonPressed(Buttons button) 
        {
            switch (button)
            {
                case Reset: return digitalRead(_resetPin) == HIGH;
                default: return false;
            }
        };
    private:
        byte _resetPin;
};

#endif