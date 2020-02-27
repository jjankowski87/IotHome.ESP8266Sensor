#ifndef IotHubClient_h
#define IotHubClient_h

#include "ConfigurationFile.h"
#include "iotc/common/string_buffer.h"
#include "iotc/iotc.h"

class IotHubClient
{
    public:
        bool initialize(Config config);
        bool sendMessage(const char* sensor, float value, Config config);
    private:
        IOTContext _context = NULL;

        static void onConnectionStatusChanged(IOTContext ctx, IOTCallbackInfo* callbackInfo)
        {
            bool isConnected = callbackInfo->statusCode == IOTC_CONNECTION_OK; 
            LOG_VERBOSE("Is connected ? %s (%d)", isConnected ? "YES" : "NO", callbackInfo->statusCode);
        }

        static void onMessageSent(IOTContext ctx, IOTCallbackInfo* callbackInfo)
        {
            AzureIOT::StringBuffer buffer;
            if (callbackInfo->payloadLength > 0)
            {
                buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
            }

            LOG_VERBOSE("- Message to IotHub sent. Payload => %s\n", buffer.getLength() ? *buffer : "EMPTY");
        }
};

#endif