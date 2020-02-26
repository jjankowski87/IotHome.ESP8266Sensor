#ifndef Configuration_h
#define Configuration_h

struct Config
{
  char deviceName[20] = { 0 };
  char deviceKey[200] = { 0 };

  bool isEmpty()
  {
    return deviceKey[0] == 0 || deviceName[0] == 0;
  }
};

class Configuration 
{
public:
  bool initialize();
  Config load();
  void save(char deviceName[20], char deviceKey[200]);
  void remove();
private:
  Config _config;
  static const char * const FileName;
};

#endif
