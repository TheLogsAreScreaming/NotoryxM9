#include "notoryx/board_profile.h"
#include "notoryx/spi_bus.h"
#include <cstdlib>
#include <iostream>
#include <string>

#define CHECK(expr) do { if (!(expr)) { std::cerr << __LINE__ << ": " #expr "\n"; std::exit(1); } } while (false)
using namespace notoryx;

struct FakeSpi : SpiPort {
  std::string log;
  bool beginOk = true, transferOk = true;
  SpiSettings settings{};
  void deselectAll() override { log += "D"; }
  bool begin(SpiSettings s) override { settings = s; log += "B"; return beginOk; }
  void select(SpiDevice d) override { log += d == SpiDevice::radio ? "R" : d == SpiDevice::display ? "L" : "S"; }
  bool transfer(const std::uint8_t*, std::uint8_t*, std::size_t) override { log += "T"; return transferOk; }
  void end() override { log += "E"; }
};

void profileTests() {
  HardwareProfile p{};
  CHECK(validate(p) == Validation::revisionUnknown);
  p.revision = Revision::v1_0;
  CHECK(validate(p) == Validation::bandUnknown);
  p.band = Band::mhz915;
  CHECK(validate(p) == Validation::keyboardUnknown);
  p.keyboard = Keyboard::stc8h;
  CHECK(validate(p) == Validation::powerUnknown);
  p.peripheralEnable = Level::low; p.gnssEnable = Level::low;
  CHECK(validate(p) == Validation::keyboardAddressInvalid);
  p.keyboardAddress = 0x6c;
  CHECK(validate(p) == Validation::gnssBaudUnsupported);
  p.gnssBaud = 115200;
  CHECK(validate(p) == Validation::recoveryUnverified);
  p.recoveryVerified = true;
  CHECK(validate(p) == Validation::pinMapUnverified);
  p.pinMapVerified = true;
  CHECK(validate(p) == Validation::radioPathUnverified);
  p.radioPathVerified = true;
  CHECK(validate(p) == Validation::ready);
  // An alternate address is not evidence of revision, power polarity or band.
  HardwareProfile unknown{}; unknown.keyboardAddress = 0x6d;
  CHECK(validate(unknown) == Validation::revisionUnknown);
  p.keyboardAddress = 0x78;
  CHECK(validate(p) == Validation::keyboardAddressInvalid);
}

void busTests() {
  FakeSpi port; SpiBus bus(port); std::uint8_t byte = 0;
  {
    SpiBus::Transaction radio(bus, SpiDevice::radio, {8000000, 0});
    CHECK(radio);
    CHECK(port.log == "DBR");
    {
      SpiBus::Transaction display(bus, SpiDevice::display, {40000000, 0});
      CHECK(!display);
      CHECK(!display.transfer(&byte, nullptr, 1));
    }
    CHECK(port.log == "DBR"); // Failed nested acquisition cannot release radio.
    CHECK(!radio.transfer(&byte, nullptr, 4097));
    CHECK(!radio.transfer(nullptr, nullptr, 1));
    CHECK(!radio.transfer(&byte, nullptr, 0));
    CHECK(radio.transfer(&byte, nullptr, 1));
  }
  CHECK(port.log == "DBRTDE");
  {
    SpiBus::Transaction display(bus, SpiDevice::display, {40000000, 3});
    CHECK(display);
    CHECK(port.settings.hz == 40000000 && port.settings.mode == 3);
    port.transferOk = false;
    CHECK(!display.transfer(&byte, nullptr, 1));
  }
  CHECK(port.log == "DBRTDEDBLTDE"); // Cleanup occurs after transfer failure.
  port.log.clear(); port.beginOk = false;
  { SpiBus::Transaction sd(bus, SpiDevice::sd, {400000, 0}); CHECK(!sd); }
  CHECK(port.log == "DB"); // No device selected when begin fails.
  port.beginOk = true; port.log.clear();
  { SpiBus::Transaction sd(bus, SpiDevice::sd, {400000, 0}); CHECK(sd); }
  CHECK(port.log == "DBSDE");
  port.log.clear();
  { SpiBus::Transaction bad(bus, SpiDevice::sd, {0, 0}); CHECK(!bad); }
  { SpiBus::Transaction bad(bus, SpiDevice::sd, {1000000, 4}); CHECK(!bad); }
  CHECK(port.log.empty());
}

int main() {
  profileTests(); busTests();
  std::cout << "Board validation and SPI ownership/failure-path tests passed\n";
}
