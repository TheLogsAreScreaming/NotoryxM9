#pragma once
#include <cstddef>
#include <cstdint>

namespace notoryx {
enum class SpiDevice { radio, display, sd };
struct SpiSettings { std::uint32_t hz; std::uint8_t mode; };

// M2 implements this port with the ESP32 SPI driver. All three peripherals
// must use it. ISRs only enqueue work; no bus access from interrupts.
class SpiPort {
 public:
  virtual ~SpiPort() = default;
  virtual void deselectAll() = 0;
  // On failure, leave no transaction/lock held and all devices deselected.
  virtual bool begin(SpiSettings settings) = 0;
  virtual void select(SpiDevice device) = 0;
  virtual bool transfer(const std::uint8_t* tx, std::uint8_t* rx, std::size_t size) = 0;
  virtual void end() = 0;
};

// Single execution owner, not a cross-task mutex. Synchronous transfers only.
// Multiple bounded transfers can form one device command, but yield between
// transactions. M2 must measure total transaction duration under load.
class SpiBus {
 public:
  static constexpr std::size_t maxTransfer = 4096;
  explicit SpiBus(SpiPort& port) : port_(port) {}
  SpiBus(const SpiBus&) = delete;
  SpiBus& operator=(const SpiBus&) = delete;

  class Transaction {
   public:
    Transaction(SpiBus& bus, SpiDevice device, SpiSettings settings) : bus_(bus) {
      if (bus_.active_ || settings.hz == 0 || settings.mode > 3) return;
      bus_.port_.deselectAll();
      if (!bus_.port_.begin(settings)) return;
      bus_.active_ = true;
      acquired_ = true;
      bus_.port_.select(device);
    }
    ~Transaction() {
      if (!acquired_) return;
      bus_.port_.deselectAll();
      bus_.port_.end();
      bus_.active_ = false;
    }
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    explicit operator bool() const { return acquired_; }
    bool transfer(const std::uint8_t* tx, std::uint8_t* rx, std::size_t size) {
      return acquired_ && size > 0 && size <= maxTransfer && (tx || rx) &&
             bus_.port_.transfer(tx, rx, size);
    }
   private:
    SpiBus& bus_;
    bool acquired_ = false;
  };
 private:
  SpiPort& port_;
  bool active_ = false;
};
}  // namespace notoryx
