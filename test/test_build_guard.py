"""Exercise the SCons guard in isolation; never invoke an upload command."""
from pathlib import Path
import runpy
import unittest

SCRIPT = Path(__file__).resolve().parents[1] / "scripts/build_only.py"


class BuildGuardTests(unittest.TestCase):
    def run_guard(self, targets):
        runpy.run_path(str(SCRIPT), init_globals={
            "Import": lambda name: None,
            "COMMAND_LINE_TARGETS": targets,
        })

    def test_build_targets_allowed(self):
        for target in ([], ["buildprog"], ["clean"], ["size"]):
            with self.subTest(target=target):
                self.run_guard(target)

    def test_hardware_targets_rejected(self):
        for target in ("upload", "uploadfs", "uploadfsota", "erase", "program", "debug"):
            with self.subTest(target=target):
                with self.assertRaisesRegex(RuntimeError, "M1 is build-only"):
                    self.run_guard(["buildprog", target])


if __name__ == "__main__":
    unittest.main()
