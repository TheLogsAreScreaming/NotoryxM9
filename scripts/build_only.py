"""M1 supports compilation, not hardware operations or flashable releases."""
Import("env")  # noqa: F821 - supplied by PlatformIO/SCons

blocked = {"upload", "uploadfs", "uploadfsota", "erase", "program", "debug"}
if blocked.intersection(COMMAND_LINE_TARGETS):  # noqa: F821
    raise RuntimeError("M1 is build-only. Hardware bring-up requires a separately reviewed M2 target.")

