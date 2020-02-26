#ifndef Led_h
#define Led_h

#include <AUnit.h>
#include "Configuration.h"

test(ShouldReturnEmptyConfiguration_WhenRemoved) {
  Configuration configuration;

  configuration.remove();
  Config config = configuration.load();

  assertTrue(config.isEmpty());
}

test(ShouldReturnUpdatedConfiguration_WhenSaved) {
  Configuration configuration;

  configuration.remove();
  configuration.save("some name", "some key");
  Config config = configuration.load();

  assertEqual(config.deviceName, "some name");
  assertEqual(config.deviceKey, "some key");
}

#endif
