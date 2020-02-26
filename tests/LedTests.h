#ifndef LedTests_h
#define LedTests_h

#include <AUnit.h>
#include "../src/Led.h"
#include "../src/Constants.h"

test(ShouldSetHighStateToLeds_WhenClassIsIstantiated)
{
    Led led(GREEN_LED_PIN, RED_LED_PIN);

    int greenState = digitalRead(GREEN_LED_PIN);
    int redState = digitalRead(RED_LED_PIN);

    assertEqual(greenState, HIGH);
    assertEqual(redState, HIGH);
}

test(ShouldSetLowStateToGreenLed_WhenBlinkingGreen)
{
    Led led(GREEN_LED_PIN, RED_LED_PIN);

    led.blink(LedColor::Green, 0.2);
    int highState = digitalRead(GREEN_LED_PIN);
    delay(200);
    int lowState = digitalRead(GREEN_LED_PIN);
    led.off(LedColor::Green);

    assertEqual(highState, HIGH);
    assertEqual(lowState, LOW);
}

test(ShouldSetLowStateToRedLed_WhenBlinkingRed)
{
    Led led(GREEN_LED_PIN, RED_LED_PIN);

    led.blink(LedColor::Red, 0.2);
    int highState = digitalRead(RED_LED_PIN);
    delay(200);
    int lowState = digitalRead(RED_LED_PIN);
    led.off(LedColor::Red);

    assertEqual(highState, HIGH);
    assertEqual(lowState, LOW);
}

test(ShouldSetHighStateToGreenLed_WhenTurningOffGreen)
{
    Led led(GREEN_LED_PIN, RED_LED_PIN);

    led.blink(LedColor::Green, 2);
    led.off(LedColor::Green);

    int highState = digitalRead(GREEN_LED_PIN);

    assertEqual(highState, HIGH);
}

test(ShouldSetHighStateToRedLed_WhenTurningOffRed)
{
    Led led(GREEN_LED_PIN, RED_LED_PIN);

    led.blink(LedColor::Red, 2);
    led.off(LedColor::Red);

    int highState = digitalRead(RED_LED_PIN);

    assertEqual(highState, HIGH);
}

#endif
