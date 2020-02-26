#ifndef ConfigurationTests_h
#define ConfigurationTests_h

#include <AUnit.h>
#include "../src/ConfigurationFile.h"

test(ShouldReturnTrue_WhenConfigurationFileIsIstantiated)
{
  ConfigurationFile configurationFile;

  assertTrue(configurationFile.isMounted());
}

test(ShouldReturnEmptyConfiguration_WhenRemoved)
{
  ConfigurationFile configurationFile;

  configurationFile.save("some name", "some key");
  configurationFile.remove();
  Config config = configurationFile.load();

  assertTrue(config.isEmpty());
}

test(ShouldReturnUpdatedConfiguration_WhenSaved)
{
  ConfigurationFile configurationFile;

  configurationFile.save("some name", "some key");
  Config config = configurationFile.load();

  assertFalse(config.isEmpty());
  assertEqual(config.deviceName, "some name");
  assertEqual(config.deviceKey, "some key");
}

#endif
