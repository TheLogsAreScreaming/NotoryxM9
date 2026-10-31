# Delivery milestones

| Gate | Scope | Acceptance | Status |
|---|---|---|---|
| M0 | Evidence, source selection, recovery plan | Revision contradictions resolved or isolated as unknown | Research recorded; hardware gate pending |
| M1 | Build foundation, board contracts, CI and host tests | ESP32 build and host tests pass with source/resource report | Implemented; see validation record |
| M2 | Approved bench diagnostic target and adapters | Display, keyboard, radio, GNSS, recovery and storage validated | Not started; hardware approval required |
| M3 | MeshCore runtime and standalone text terminal | Two-way direct/channel/repeater tests and truthful delivery state | Not started |
| M4 | NOTORYX UI and persistence | Five destinations operable without phone; power-loss tests | Not started |
| M5 | Check-ins, signed posts, SOS and position tools | App interoperability, cancellation, stale-fix and loss tests | Not started |
| M6 | Release candidate | Soak, current measurement, recovery and interrupted-update tests | Not started |

## Scope

First usable release: keyboard text messaging, discovery, contacts, direct and
channel conversations, drafts/history, radio setup, battery/GNSS/diagnostics,
brightness and sound. Position sharing must show fix freshness.

Next: check-ins, explicit SOS activation/cancellation/all-clear, signed posts,
repeater ping, breadcrumbs and export, optional companion BLE/USB.

Later: offline maps, compass calibration, advanced routes, Wi-Fi maintenance,
OTA and broader language support. MQTT, media/GIFs, AI and full app parity are
outside the initial scope. Start as a client, not a repeater.

## Risks

Revision mismatch is gated by explicit evidence. SPI contention needs measured
transfer budgets. Persistence needs atomic/versioned records and no automatic
format. Sleep requires reception/wake verification. Signing needs byte fixtures
and cross-tests. Build success is not a radio or battery-life result.

