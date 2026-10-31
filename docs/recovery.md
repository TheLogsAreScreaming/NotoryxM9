# Recovery gate — not a flashing guide

No hardware was accessed for M0/M1. Do not publish offsets or a button sequence
until verified on the actual unit. Compilation does not establish compatibility.

ESP32-S3 ROM download requires GPIO0 low on reset and GPIO46 low or floating.
See [Espressif boot selection](https://docs.espressif.com/projects/esptool/en/latest/esp32s3/advanced-topics/boot-mode-selection.html).
The manual identifies Reset, not a complete documented M9 BOOT sequence.
The USB-C connector alone does not prove native USB download wiring.

Before a separately approved M2 flash:

1. Record board revision, RF SKU, factory version, serial bridge and reset path.
2. Verify ROM access independently of the running application; record the exact
   physical procedure. Avoid confusing main-controller and keyboard boot modes.
3. Read flash identity, partition table and security state with approved tools.
   Do not change eFuses, secure boot, flash encryption or anti-rollback settings.
4. Preserve factory recovery image/settings and identity through a protected
   backup where supported. A raw encrypted dump is not automatically restorable.
5. Validate image type, partition sizes, chip ID, offsets and checksums against
   the generated build and the device. Application-only and merged images differ.
6. Review reliable power and recovery procedure before performing the flash.
7. Test restoration on the approved test unit before declaring recovery proven.

Keep keyboard firmware and LR1110 internal firmware updates out of initial
bring-up. No autoformat, mass erase, OTA or production release in M1.

