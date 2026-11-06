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
repeater ping, breadcrumbs and export, optional companion BLE/USB, and a
receive-only "also hear Meshtastic" setting (see below).

Later: offline maps, compass calibration, advanced routes, Wi-Fi maintenance,
OTA and broader language support. MQTT, media/GIFs, AI and full app parity are
outside the initial scope. Start as a client, not a repeater.

## v1.x: also hear Meshtastic (receive only)

Approved 2026-11-06 for after the first usable release (M3/M4); not part of it.

- Setting OFF/ON, default OFF. Public Meshtastic LongFast only, shown in its
  own tab: sender, hops, RSSI/SNR, text and position. The M9 never transmits
  Meshtastic; sending stays MeshCore only. Private channels and PKI DMs stay
  unread.
- Method: stay in MeshCore receive (short ~65 ms preamble on the US preset)
  and, only while MeshCore is idle (not sending, not receiving, not in
  listen-before-talk), take a short CAD look at LongFast (~165 ms preamble)
  about every 30 ms, returning to MeshCore unless a preamble is found. Lives
  in our LR1110 radio wrapper; the radio is restored exactly as MeshCore left
  it after every look.
- M2 measurement first: LR1110 retune + 2-symbol LongFast CAD time and how
  soon preamble-detected follows a hit. The plan only holds if a look stays
  well under the MeshCore preamble (~60 ms budget, including any SPI wait).
- Acceptance: with a second node sending 25 messages on each network, at
  least 24/25 received on both with the setting ON; with it OFF, MeshCore
  receive is unchanged. Host tests for the Meshtastic decoder (header,
  AES-CTR on the default key, minimal hand-written protobuf reader).
- Licence: MIT. The approach was learned from a GPL-3.0 project
  (SquachWatch-CYD); no code from it, and no vendored Meshtastic protobufs
  (also GPL). Protocol facts only, written fresh.
- Open: whether to also listen to MediumFast (913.125 MHz in the US), which
  would halve the timing margin.

## Risks

Revision mismatch is gated by explicit evidence. SPI contention needs measured
transfer budgets (also the v1.x Meshtastic look). Persistence needs
atomic/versioned records and no automatic format. Sleep requires
reception/wake verification. Signing needs byte fixtures and cross-tests.
Build success is not a radio or battery-life result.
