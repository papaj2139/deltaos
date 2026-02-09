#!/usr/bin/env python3
import tomllib
from pathlib import Path

DRIVER_DIR = Path("drivers")

# Load registry
registry = tomllib.loads((DRIVER_DIR / "registry.toml").read_text())["drivers"]

# Scan for all .c files under kernel/drivers/
driver_files = {
    f.stem: f.relative_to(DRIVER_DIR)
    for f in DRIVER_DIR.rglob("*.c")
}

# Match registry keys to discovered files
enabled_sources = []
enabled_names = []
for name, enabled in registry.items():
    if enabled:
        if name not in driver_files:
            raise SystemExit(f"Error: driver '{name}' not found in drivers directory")
        enabled_sources.append(str(driver_files[name]))
        enabled_names.append(name)

# Write Makefile fragment
mk_out = DRIVER_DIR / "drivers_enabled.mk"
with mk_out.open("w") as f:
    f.write("# Auto-generated. Do not edit.\n")
    f.write("DRIVERS := " + " ".join(enabled_sources) + "\n")

# Write C header
header_out = DRIVER_DIR / "drivers_enabled.h"
with header_out.open("w") as f:
    f.write("// Auto-generated. Do not edit.\n")
    f.write("#ifndef DRIVERS_ENABLED_H\n#define DRIVERS_ENABLED_H\n\n")
    for name in registry:
        macro = f"DRIVER_{name.upper()}"
        val = "1" if name in enabled_names else "0"
        f.write(f"#define {macro} {val}\n")
    f.write("\n#endif")
