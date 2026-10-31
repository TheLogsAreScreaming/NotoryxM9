# Architecture decision: standalone client

Use C++17 with PlatformIO/Arduino-ESP32, initially aligned with the selected
MeshCore toolchain. NOTORYX Core remains a separate Flutter companion app.
Do not attempt to run Flutter or its mobile plugins on the ESP32-S3.

## Execution and shared buses

One execution context owns MeshCore and all state changes. Interrupts notify
that owner; they do not draw, write storage, or call SPI. UI actions become
bounded commands; received mesh events update bounded application models.

`SpiBus` is a synchronous single-owner contract, **not a mutex**. M2's concrete
ESP32 adapter must make RadioLib, LCD and SD use the same bus path. A successful
transaction deselects all devices, installs its settings, selects one device,
then deselects before ending. Failed nested acquisitions do not release the
current owner. Transfers are capped at 4096 bytes. This does not limit aggregate
transaction time: M2 must split redraws/SD work and measure worst-case radio
service latency, including DMA completion before ownership release.

## Layers

1. Board: explicit validated revision profile, rails, keyboard, ST7789, LR1110,
   GNSS UART, RTC and battery measurement.
2. Mesh: retain upstream identity, encryption, packet handling and routes.
   Adapt `BaseChatMesh` and companion `MyMesh`/`DataStore`; do not rewrite crypto.
3. Services: conversations, delivery state, outbox, position freshness, settings.
4. UI: Home, Chats, Nodes, Position/Map and More; focus navigation and text entry.
5. Persistence: versioned records, atomic settings and bounded journal; optional SD.

M1 contains contracts and a serial-only startup, not implementations of these
five layers. Candidate hardware profiles require all evidence fields; even a
`ready` profile is not a legal-region or RF-parameter validation. M2/M3 must add
separate region, frequency, antenna, power and network checks before TX.

## Integration choices

Keep upstream source attributable, pinned and minimally patched. Review
`BaseChatMesh` send/receive/ack callbacks and companion persistence before import.
The local application uses direct commands and callbacks, not a simulated BLE
link. Optional BLE/USB clients later share the same state owner.

Use the existing diagnostic renderer for bring-up; evaluate LVGL with bounded
partial buffers for the application. RGB565 full frame = 320*240*2 = 153600
bytes. No full-screen blur. PSRAM availability does not eliminate internal
heap, DMA or SPI bandwidth constraints. Establish budgets from measurements.

Never autoformat valuable storage after a mount failure. Upstream companion
`SPIFFS.begin(true)` needs adaptation. Keep secrets out of logs/SD exports.
Screen-off is distinct from deep sleep; do not promise receive availability
while sleeping until tested. No repeater role by default.

## Compatibility

Test standard MeshCore advertisements, direct and channel messages with stock
peers and a repeater, then separately test companion BLE/USB compatibility.
Signed posts later preserve NOTORYX's `0xAE51` GRP_DATA payload, canonical
signing input, 101-byte encoding, and out-of-order text/signature handling.
Convert test vectors from the pinned app revision and cross-test on air.
Queued, sent, acknowledged and expired are different states; channel sends
never imply every recipient received the message.

