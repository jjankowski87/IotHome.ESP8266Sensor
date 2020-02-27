#include "IotHubClient.h"

bool IotHubClient::initialize(Config config)
{
    int errorCode = iotc_init_context(&_context);
    if (errorCode != 0)
    {
        LOG_ERROR("Error initializing IOTC. Code %d", errorCode);
        return false;
    }

    iotc_set_logging(IOTC_LOGGING_API_ONLY);

    iotc_on(_context, "ConnectionStatus", IotHubClient::onConnectionStatusChanged, NULL);
    iotc_on(_context, "MessageSent", IotHubClient::onMessageSent, NULL);

    errorCode = iotc_connect(_context, "", config.deviceKey, "", IOTC_CONNECT_CONNECTION_STRING);

    if (errorCode != 0)
    {
        LOG_ERROR("Error @ iotc_connect. Code %d", errorCode);
        return false;
    }

    return true;
}

bool IotHubClient::sendMessage(const char* sensor, float value, Config config)
{
    int pos = 0;
    int errorCode = 0;

    char msg[80] = {0};
    int val_int = (int) value;
    int val_fra = (int) ((value - (float)val_int) * 100);
    pos = snprintf(msg, sizeof(msg) - 1, "{\"Sensor\": \"%s\", \"Name\": \"%s\", \"Value\": %d.%d}", sensor, config.deviceName, val_int, val_fra);

    iotc_send_telemetry(_context, msg, pos);

    if (errorCode != 0)
    {
        LOG_ERROR("Sending message has failed with error code %d", errorCode);
        return false;
    }

    return true;
}

