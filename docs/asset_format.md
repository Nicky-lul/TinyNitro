# TinyNitro Asset Format

TinyNitro packs assets into `.tnpak` files.

## File Layout

```text
TNPackHeader
TNAssetEntry[]
Extra metadata blocks
Raw asset data blocks
```

## Header

```c
typedef struct {
    char magic[4];      // "TNPK"
    u16 version;        // 1
    u16 assetCount;
    u32 tableOffset;
    u32 dataOffset;
} TNPackHeader;
```

## Asset Entry

```c
typedef struct {
    char name[32];
    u16 type;           // 1 audio, 2 image, 3 map
    u16 format;         // format depends on type
    u32 offset;         // raw asset data offset
    u32 size;           // raw asset data size
    u32 extraOffset;    // metadata offset, or 0
} TNAssetEntry;
```

## Audio Extra

```c
typedef struct {
    u32 sampleRate;
    u16 channels;
    u16 format;
    u32 totalSamples;
    u32 chunkSize;
    u32 chunkCount;
    u32 flags;
} TNAudioExtra;
```

## Audio Flags

```text
1 = loop
```

## Current Audio Format

```text
PCM8 = unsigned 8-bit mono PCM
```

ADPCM is reserved for a later version.
