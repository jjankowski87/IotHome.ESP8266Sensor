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
    Setup,
    Waiting,
    Initializing,
    Sending,
    Error,
    Finished,
    Unknown
};

enum ReadingType
{
    Temperature,
    Humidity
};

#endif
