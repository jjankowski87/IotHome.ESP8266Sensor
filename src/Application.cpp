#include "Application.h"
#include "Constants.h"
#include "TimeHelper.h"
#include "iotc/iotc.h"

Application::Application()
{
    _led = new Led(GREEN_LED_PIN, RED_LED_PIN);
    _keyboard = new Keyboard(RESET_BUTTON_PIN);
    _configurationFile = new ConfigurationFile();
    _wifi = new Connection(_configurationFile, _led);
    _sensor = new EnvironmentalSensor(BME_ADDRESS);
    _iotHubClient = new IotHubClient();

    _startupTime = millis();
    _appState = AppState::Setup;
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
        case ReadSensor: return readSensor();
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

    return AppState::ReadSensor;
}

AppState Application::readSensor()
{
    _temperature = _sensor->get(ReadingType::Temperature);
    _humidity = _sensor->get(ReadingType::Humidity);

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
    Config config = _configurationFile->load();

    bool isTemperatureSent = _iotHubClient->sendMessage("temperature", _temperature, config);
    bool isHumiditySent = _iotHubClient->sendMessage("humidity", _humidity, config);

    if (!isTemperatureSent || !isHumiditySent)
    {
        return AppState::Error;
    }

    _led->blink(LedColor::Green, BLINK_SEND);
    delay(1200);

    return AppState::Finished;
}

AppState Application::error()
{
    _led->blink(LedColor::Red, BLINK_ERROR);
    delay(1200);
    _led->off(LedColor::Red);

    ESP.deepSleep(TimeHelper::toUs(RESTART_SLEEP_TIME));

    return AppState::Unknown;
}

AppState Application::finished()
{
    _led->off(LedColor::Green);
    _led->off(LedColor::Red);

    long sleepTime = _wifi->getSleepTime(_startupTime);
    ESP.deepSleep(sleepTime);

    return AppState::Unknown;
}