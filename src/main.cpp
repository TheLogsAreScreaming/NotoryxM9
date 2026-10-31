#include <Arduino.h>
#include "notoryx/board_profile.h"
#include "notoryx/spi_bus.h"

void setup() {
  Serial.begin(115200);
  Serial.println("NOTORYX M9 | M1 build foundation | NOT hardware validated");
  Serial.printf("Flash: %u bytes; PSRAM: %u bytes; free heap: %u bytes\n",
                ESP.getFlashChipSize(), ESP.getPsramSize(), ESP.getFreeHeap());
  constexpr notoryx::HardwareProfile profile{};
  static_assert(notoryx::validate(profile) != notoryx::Validation::ready,
                "Unknown hardware must not be activated");
  Serial.println("Peripherals inactive; board revision and recovery validation pending.");
}

void loop() {
  delay(1000);
}
