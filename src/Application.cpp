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
    _sensor = new BmeSensor(BME_ADDRESS);
    _iotHubClient = new IotHubClient();
}

Application::~Application()
{
    delete _configurationFile;
    delete _wifi;
    delete _keyboard;
    delete _led;
    delete _sensor;
    delete _iotHubClient;
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
    if (!_configurationFile->isMounted() || !_sensor->isConnected())
    {
        return AppState::Error;
    }

    _led->on(LedColor::Green);
    return AppState::Waiting;
}

AppState Application::wait()
{
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

    Config config = _configurationFile->load();
    if (!_iotHubClient->initialize(config))
    {
        return AppState::Error;
    }

    return AppState::Sending;
}

AppState Application::send()
{
    _led->blink(LedColor::Green, BLINK_SEND);

    Config config = _configurationFile->load();
    _iotHubClient->sendMessage("temperature", _sensor->get(ReadingType::Temperature), config);
    _iotHubClient->sendMessage("humidity", _sensor->get(ReadingType::Humidity), config);

    return AppState::Finished;
}

AppState Application::error()
{
    _led->blink(LedColor::Red, BLINK_ERROR);
    delay(1200);

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