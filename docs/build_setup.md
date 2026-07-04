# Build Setup

## PC Compiler

Requires Python 3.

Run from the repo root:

```bash
python tools/tinynitro_compiler/main.py examples/horror_room/project.tnproj -o runtime/nds/data/game.tnpak
```

The example project expects these files to exist:

```text
examples/horror_room/assets_src/sounds/pickup.wav
examples/horror_room/assets_src/sounds/ambience.wav
```

## DS Runtime

Install devkitPro, devkitARM, libnds, and libfat.

Then run:

```bash
cd runtime/nds
make
```

Output:

```text
tinynitro.nds
```

## Emulator Notes

The runtime currently loads `/data/game.tnpak` through FAT. For emulator testing, use an emulator/cart setup that supports FAT filesystem access.

## Hardware Notes

The current audio code is a prototype. It should be treated as a first test layer, not final seamless streaming.
