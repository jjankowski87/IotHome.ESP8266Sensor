#ifndef ConfigurationFile_h
#define ConfigurationFile_h

#include <ArduinoJson.h>

struct Config
{
    char deviceName[20] = { 0 };
    char deviceKey[200] = { 0 };
    char serviceUrl[200] = { 0 };

    bool isEmpty()
    {
        return deviceKey[0] == 0 || deviceName[0] == 0 || serviceUrl[0] == 0;
    }
};

class ConfigurationFile 
{
    public:
        ConfigurationFile();

        inline bool isMounted() { return _isMounted; };
        Config load();
        void save(char deviceName[20], char deviceKey[200], char serviceUrl[200]);
        void remove();
    private:
        bool _isMounted;
        static const char * const ConfigFileName;
};

#endif
