# Hardware evidence and validation matrix

Status: documentation/source inspection only. No unit, revision, RF band or
recovery path has been verified. M0 research is recorded; its bench gate is open.

## Sources

- [Elecrow MeshCore M9 wiki](https://www.elecrow.com/wiki/ThinkNode_M9_MeshCore_Communication_Terminal_with_Full_Keyboard.html)
- [Elecrow manual](https://www.elecrow.com/download/product/LMM16509D/ThinkNode_M9_for_Meshcore_User_Manual.pdf)
- [Band-specific SKUs](https://www.elecrow.com/blog/thinknode-m9-product-selection-and-overview.html)
- [Pinned upstream board source](https://github.com/meshcore-dev/MeshCore/tree/81dbae592ffad40b4212bc0b37e82b38da08f4bc/variants/thinknode_m9)
- [SolarOS M9 manifest](https://github.com/nilseuropa/solar_os/blob/main/boards/manifests/thinknode_m9.toml)
- [SolarOS keyboard implementation](https://github.com/nilseuropa/solar_os/blob/main/src/services/solar_os_stc8h_keyboard.c)

SolarOS is corroborating implementation evidence, not authoritative proof of
the user's hardware. Its source was inspected, not copied. Obtain a
revision-matched original schematic; only published derived pin tables were
available during this research.

## Candidate main-controller pin map

| Function | GPIO |
|---|---|
| SPI SCK / MOSI / MISO | 40 / 47 / 38 |
| LR1110 CS / reset / BUSY / IRQ | 39 / 45 / 41 / 42 |
| LCD CS / DC / reset / backlight / TE | 16 / 15 / 14 / 17 / 19 |
| SD CS | 48 |
| Keyboard SDA / SCL / wake | 20 / 21 / 12 |
| RTC/sensors SDA / SCL | 7 / 6 |
| GNSS RX / TX (ESP32 perspective) | 2 / 3 |
| GNSS PPS / reset / standby / enable | 4 / 5 / 10 / 11 |
| Peripheral enable / battery ADC / buzzer | 18 / 13 / 9 |
| Charge status / external power / BOOT | 8 / 1 / 0 |

Expected configuration: ESP32-S3R8, 16 MB flash, 8 MB octal PSRAM, ST7789
240x320 panel rotated to landscape. Memory remains to be measured on the unit.
868/915 MHz hardware SKUs are not interchangeable solely through settings.

## Unresolved gates

| Issue | Evidence | Required resolution |
|---|---|---|
| Keyboard identity | Wiki: ESP32-S2; SolarOS: measured STC8H | Confirm actual controller/protocol; preserve factory firmware |
| Keyboard addresses | SolarOS reports 0x6c/0x6d | Capture key events; address alone cannot set revision |
| GNSS power | Upstream active-low; SolarOS reports v1.1 active-high | Confirm polarity before driving enable |
| GNSS baud | variant.h 9600; platformio.ini 115200 | Identify actual NMEA baud and receiver |
| GNSS direction/model | RX/TX macros differ; variant says L76K | Trace effective Serial1 setup; avoid unverified vendor commands |
| Radio IRQ helper | getIRQGpio returns BUSY alias 41; constructor uses 42 | Audit callers, verify interrupt 42 |
| Battery GPIO | PIN_LED aliases battery ADC 13 | Never drive as generic status LED |
| GPIO46 | Listed as keyboard LED; also boot strap | Verify schematic/drive capability before using; omitted from driver contract |
| RF setup | TCXO 3.3 V and DIO5/DIO6 table in target.cpp | Verify selected board, LR1110 version and two-way RF |
| USB path | CH340K; GPIO19/20 also used by peripherals | Verify UART0/reset wiring; no assumed native USB |

## Bench checklist (all pending, requires hardware approval)

- [ ] Record PCB revision, SKU, controller markings, GNSS module and factory version.
- [ ] Obtain schematic and confirm safe rail polarities and reset levels.
- [ ] Verify ROM recovery and preserve factory configuration/identity securely.
- [ ] Measure flash/PSRAM and record partitions/security state.
- [ ] Test CS exclusivity with logic analyzer during overlapping workloads.
- [ ] Verify LCD geometry, RGB order, inversion, brightness and full-screen corners.
- [ ] Exercise every key, symbol, shortcut, long press, repeat and wake transition.
- [ ] Verify LR1110 version, BUSY, IRQ, TCXO and RF switch; start with receive tests.
- [ ] Test direct/channel messaging both ways and through a known-good repeater.
- [ ] Receive continuously while typing, refreshing the screen and writing SD.
- [ ] Verify GNSS cold/warm starts, NMEA checksums, stale fixes and power cycles.
- [ ] Calibrate battery ADC; verify charging status and USB/battery transitions.
- [ ] Verify RTC retention, absent/full/removed SD, interrupted writes and reboots.
- [ ] Measure idle/screen-off/GNSS/receive/sleep current and all wake sources.

Record results with unit/revision, source commit, instrument/setup, measured
values, pass/fail and logs. No unchecked item is implied to pass by a build.

