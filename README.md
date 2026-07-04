# TinyNitro

TinyNitro is a tiny Nintendo DS/DSi homebrew runtime plus a PC asset compiler.

The goal is to take normal assets like WAV and PNG files, downscale them on PC, pack them into DS-friendly formats, and run them from a small DS runtime.

## Current Prototype

This starter version includes:

- WAV to unsigned 8-bit mono PCM conversion
- Optional audio downsampling
- Chunk metadata for long streamed audio
- A simple `.tnpak` asset pack format
- A Nintendo DS runtime skeleton using devkitPro/libnds
- Pack loading structures and test hooks

## Folder Layout

```text
tools/tinynitro_compiler/   PC-side asset compiler
runtime/nds/                Nintendo DS runtime project
examples/horror_room/       Example TinyNitro project
docs/                       Format and pipeline notes
```

## Build an Asset Pack

```bash
python tools/tinynitro_compiler/main.py examples/horror_room/project.tnproj -o runtime/nds/data/game.tnpak
```

## Build the DS Runtime

Install devkitPro with devkitARM and libnds first.

```bash
cd runtime/nds
make
```

## Current Milestone

```text
Press A on DS -> play a packed test sound
Press B on DS -> play the next ambience chunk
```

## Status

Early prototype. The PC compiler is usable for WAV-to-PCM8 packing. The DS runtime is a skeleton and needs testing in a DS emulator or on real hardware.
