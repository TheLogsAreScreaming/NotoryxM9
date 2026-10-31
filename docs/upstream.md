# Reuse inventory

See `upstream.lock.json` for selected immutable revisions. Refresh them only
with review, compatible toolchain checks and regression tests.

| Source | M1 use | Later use |
|---|---|---|
| MeshCore boards/thinknode_m9.json | Adapted memory/CPU/framework flags; generic esp32s3 framework variant; misleading M1 product URL removed | Verified board target |
| MeshCore variants/thinknode_m9 | Pin/evidence reference; contradictions retained in hardware matrix | Board initialization and LR1110 RF setup after audit |
| MeshCore src + companion example | Architectural reference only | Protocol, crypto, identity, routing, contacts and persistence adapters |
| RadioLib selected commit | Recorded, not downloaded or linked | LR1110 driver |
| NOTORYX Core | Product reference, no Dart source copied | Themes, presets, field-state behavior and protocol fixtures |
| SolarOS M9 source | Evidence only, no source copied | Independently validate keyboard protocol before driver choice |

NOTORYX Core's rc.22 source was selected after PR #181/rc.21; the project had
advanced while research was in progress. This firmware is a separate versioned
product and does not inherit the Flutter app's release number or test status.

For signed posts, preserve `lib/services/signed_posts.dart` semantics and
`test/services/signed_posts_test.dart` fixtures at the selected revision.
Port only chosen field-operations state machines, not ChangeNotifier/timers or
mobile notification code. Do not copy Flutter plugins, rendering or keychains.

Preserve the MeshCore MIT notice for board-derived configuration. Future imports
must carry their own notices; do not assume all dependencies or artwork have the
same license. Factory Ripple application source/assets are not imported.

