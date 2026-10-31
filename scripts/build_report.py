"""Record development artifact provenance. This is not a flashing manifest."""
import hashlib
import json
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
commit = subprocess.run(["git", "rev-parse", "HEAD"], cwd=root, text=True,
                        capture_output=True, check=True).stdout.strip()
dirty = bool(subprocess.run(["git", "status", "--porcelain"], cwd=root, text=True,
                            capture_output=True, check=True).stdout.strip())
artifacts = {}
for name in ("firmware.bin", "firmware.elf", "bootloader.bin", "partitions.bin"):
    path = root / ".pio/build/m9_foundation" / name
    data = path.read_bytes()  # Missing build artifacts must fail the report.
    artifacts[name] = {"bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}
report = {"schema": 1, "source_commit": commit, "working_tree_dirty": dirty,
          "target": "m9_foundation", "hardware_validated": False,
          "flash_authorized": False, "distribution": "development-only",
          "upstream": json.loads((root / "upstream.lock.json").read_text()),
          "artifacts": artifacts}
out = root / "build/reports"
out.mkdir(parents=True, exist_ok=True)
(out / "build.json").write_text(json.dumps(report, indent=2) + "\n")
print(json.dumps(report, indent=2))

