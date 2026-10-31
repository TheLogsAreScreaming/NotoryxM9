# NOTORYX M9

Standalone NOTORYX firmware for the Elecrow ThinkNode M9, maintained separately
from [NOTORYX Core](https://github.com/TheLogsAreScreaming/notoryx-core).

**Status: M1 build foundation. Not usable messaging firmware. No hardware has
been validated.** This build prints startup diagnostics over UART0 and leaves
application peripherals inactive. The Arduino framework still performs normal
chip/flash/PSRAM initialization; this is not a hardware safety certification.

## Build and test

Use Python 3.11 and PlatformIO Core 6.2.0:

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements-build.txt
.venv/bin/pio run -e m9_foundation
cmake -S . -B build/host
cmake --build build/host
ctest --test-dir build/host --output-on-failure
python3 -m unittest discover -s test -p 'test_*.py'
python3 scripts/build_report.py
```

These commands compile only. PlatformIO upload, erase and debug targets are
blocked for this milestone. Hardware bring-up needs a separately reviewed M2
target and explicit flashing approval. Do not use an external flasher with
these unvalidated artifacts.

## Included

- Pinned ESP32-S3/Arduino build and M9 board description.
- Main-controller pin inventory and explicit hardware-evidence validation.
- Single-owner SPI transaction abstraction with failure-path host tests.
- CI host tests, firmware compilation and build reports; no deployment.
- [Hardware evidence](docs/hardware.md), [architecture](docs/architecture.md),
  [recovery checklist](docs/recovery.md), [milestones](docs/milestones.md), and
  [source reuse inventory](docs/upstream.md).

## Not included yet

Radio/keyboard/display/GNSS drivers, the MeshCore runtime, a standalone UI,
message persistence, power management, OTA and field features. MeshCore and
RadioLib revisions are selected in `upstream.lock.json` for later integration;
they are **not linked by this foundation build**.

The keyboard-controller identity and revision-dependent power behavior remain
unresolved. No unknown board is automatically assigned a validated profile.

## Source layout

`include/notoryx` contains portable board contracts. `src` is the embedded entry
point. `test` runs on the host. `boards` holds the compile target. `scripts`
contains build-only tooling. Hardware adapters, mesh services, UI and storage
will be introduced in their respective milestones rather than empty stubs.

This is an independent project, not an official Elecrow or MeshCore release.

