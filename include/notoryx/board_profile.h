#pragma once
#include <cstdint>

namespace notoryx {

// Main-controller GPIOs only. Keyboard matrix pins belong to another MCU.
struct M9Pins {
  static constexpr int spiSck = 40, spiMosi = 47, spiMiso = 38;
  static constexpr int radioCs = 39, radioReset = 45, radioBusy = 41, radioIrq = 42;
  static constexpr int lcdCs = 16, lcdDc = 15, lcdReset = 14, lcdBacklight = 17;
  static constexpr int lcdTe = 19, sdCs = 48;
  static constexpr int keyboardSda = 20, keyboardScl = 21, keyboardWake = 12;
  static constexpr int sensorSda = 7, sensorScl = 6;
  static constexpr int gnssRx = 2, gnssTx = 3, gnssPps = 4, gnssReset = 5;
  static constexpr int gnssStandby = 10, gnssEnable = 11;
  static constexpr int peripheralEnable = 18, batteryAdc = 13, buzzer = 9;
  static constexpr int chargeDone = 8, externalPower = 1, boot = 0;
};

enum class Revision { unknown, v1_0, v1_1 };
enum class Band { unknown, mhz868, mhz915 };
enum class Level { unknown, low, high };
enum class Keyboard { unknown, esp32s2, stc8h };

// Evidence is supplied only after bench validation. An I2C address alone
// must never select a board revision, GNSS polarity, or RF band.
struct HardwareProfile {
  Revision revision = Revision::unknown;
  Band band = Band::unknown;
  Keyboard keyboard = Keyboard::unknown;
  Level peripheralEnable = Level::unknown;
  Level gnssEnable = Level::unknown;
  std::uint8_t keyboardAddress = 0;
  std::uint32_t gnssBaud = 0;
  bool recoveryVerified = false;
  bool pinMapVerified = false;
  bool radioPathVerified = false;
};

enum class Validation {
  ready, revisionUnknown, bandUnknown, keyboardUnknown, powerUnknown,
  keyboardAddressInvalid, gnssBaudUnsupported, recoveryUnverified,
  pinMapUnverified, radioPathUnverified
};

constexpr Validation validate(const HardwareProfile& p) {
  if (p.revision == Revision::unknown) return Validation::revisionUnknown;
  if (p.band == Band::unknown) return Validation::bandUnknown;
  if (p.keyboard == Keyboard::unknown) return Validation::keyboardUnknown;
  if (p.peripheralEnable == Level::unknown || p.gnssEnable == Level::unknown)
    return Validation::powerUnknown;
  if (p.keyboardAddress < 0x08 || p.keyboardAddress > 0x77)
    return Validation::keyboardAddressInvalid;
  if (p.gnssBaud != 9600 && p.gnssBaud != 115200)
    return Validation::gnssBaudUnsupported;
  if (!p.recoveryVerified) return Validation::recoveryUnverified;
  if (!p.pinMapVerified) return Validation::pinMapUnverified;
  if (!p.radioPathVerified) return Validation::radioPathUnverified;
  return Validation::ready;
}

static_assert(M9Pins::radioIrq != M9Pins::radioBusy, "IRQ and BUSY differ");
static_assert(M9Pins::radioCs != M9Pins::lcdCs && M9Pins::radioCs != M9Pins::sdCs &&
              M9Pins::lcdCs != M9Pins::sdCs, "SPI chip selects must be distinct");
}  // namespace notoryx

