#include "Application.h"
#include "Constants.h"
#include "iotc/iotc.h"

Application::Application()
{
    _startupTime = millis();
    _appState = AppState::Setup;

    _led = new Led(GREEN_LED_PIN, RED_LED_PIN);
    _keyboard = new Keyboard(RESET_BUTTON_PIN);
    _configurationFile = new ConfigurationFile();
    _wifi = new Connection(_configurationFile, _led);
}

Application::~Application()
{
    delete _configurationFile;
    delete _wifi;
    delete _keyboard;
    delete _led;
}

void Application::loop()
{
    _appState = nextState();
}

inline AppState Application::nextState()
{
    switch (_appState)
    {
        case Setup: return setup();
        case Waiting: return wait();
        case Initializing: return initialize();
        case Sending: return send();
        case Error: return error();
        case Finished: return finished();
    }
}

AppState Application::setup()
{
    _led->on(LedColor::Green);
    return AppState::Waiting;
}

AppState Application::wait()
{
    if (!_configurationFile->isMounted())
    {
        _led->off(LedColor::Green);
        return AppState::Error;
    }

    if (_keyboard->isButtonPressed(Buttons::Reset))
    {
        _state.resetWiFi = true;
        _led->off(LedColor::Green);
        return AppState::Initializing;
    }

    if (timePassed(INIT_DELAY))
    {
        _led->off(LedColor::Green);
        return AppState::Initializing;
    }

    return AppState::Waiting;
}

AppState Application::initialize()
{
    if (!_wifi->connect(_state.resetWiFi))
    {
        return AppState::Error;
    }

    return AppState::Sending;
}

AppState Application::send()
{
    LOG_VERBOSE("send");

    _led->blink(LedColor::Green, BLINK_SEND);
    delay(5000);

    // TODO: configure bme and iothub
    //  if (!configureWiFi() || !configureBme() || !configureIoTHub()) {
    //  if (isConnected) {
    //    sendMessage("temperature", bme.readTemperature());
    //    sendMessage("humidity", bme.readHumidity());

    return AppState::Finished;
}

AppState Application::error()
{
    LOG_VERBOSE("error");

    _led->blink(LedColor::Red, BLINK_ERROR);
    delay(5000);

    return AppState::Finished;
}

AppState Application::finished()
{
    LOG_VERBOSE("finished");

    _led->off(LedColor::Green);
    _led->off(LedColor::Red);

    // TODO: deep sleep SLEEP_TIME - processing time
    // ESP.deepSleep(SLEEP_TIME);
    delay(5000);
    
    return AppState::Finished;
}