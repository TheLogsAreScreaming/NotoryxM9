# M1 validation record

Tested locally on macOS with AppleClang 21, Python 3.13.2 and PlatformIO 6.2.0.
CI specifies Ubuntu 24.04/Python 3.11 and independently repeats build/tests.
CI status must be checked on the pull request; local success is not CI success.

## Results

- CMake build with `-Wall -Wextra -Werror -pedantic`: passed.
- CTest foundation executable: passed. Covers unknown-profile rejection,
  required evidence, nested SPI acquisition, CS/transaction ordering,
  transfer failures, begin failures, invalid settings and oversized transfers.
- Python upload-guard tests: passed in isolation, without invoking any uploader.
- ESP32-S3 `m9_foundation` build: passed using the pinned cached toolchain.
- Build size: 268641 bytes flash, 18604 bytes static RAM (baseline only).

`scripts/build_report.py` records commit, dirty state, binary sizes/checksums and
upstream revisions after a build. CI retains the report, not firmware binaries.
The report is not an installation or offset manifest.

## Limits

No hardware tests, no peripheral initialization, no RF transmission, no flashing,
no image release. MeshCore runtime is not linked and no standalone messaging UI
exists in M1. The memory result is not a budget for the completed application.
M0's physical revision/recovery questions and all M2 bench checks remain pending.
