# TinyNitro Audio Pipeline

TinyNitro does heavy audio conversion on PC, not on the DS/DSi.

## V1 Pipeline

```text
WAV file
  -> mono
  -> downsample
  -> unsigned 8-bit PCM
  -> optional chunk metadata
  -> .tnpak
```

## Recommended Settings

Short SFX:

```text
11025 Hz
mono
PCM8
preloaded/read as one asset
```

Long ambience:

```text
22050 Hz
mono
PCM8
32768-byte chunks
loop enabled if needed
```

## Why PCM8 First?

PCM8 is not the best final format, but it is the easiest to debug. Once playback works, ADPCM should be added to reduce file size.

## Next Audio Goals

1. Fix sample lifetime management for SFX buffers.
2. Add proper double-buffered streaming.
3. Add ADPCM encoding on PC.
4. Add ADPCM playback on DS.
5. Add sound priority and channel management.
