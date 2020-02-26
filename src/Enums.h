#ifndef Enums_h
#define Enums_h

enum Buttons
{
    Reset
};

enum LedColor
{
    Green,
    Red
};

enum AppState
{
    Waiting,
    Initializing,
    Sending,
    Error,
    Finished
};

#endif
