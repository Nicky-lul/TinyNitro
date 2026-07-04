#ifndef TINYNITRO_FORMAT_H
#define TINYNITRO_FORMAT_H

#include <nds.h>

#define TN_VERSION 1

#define TN_ASSET_AUDIO 1
#define TN_ASSET_IMAGE 2
#define TN_ASSET_MAP 3

#define TN_AUDIO_PCM8 1
#define TN_AUDIO_ADPCM 2

#define TN_AUDIO_FLAG_LOOP 1

typedef struct {
    char magic[4];
    u16 version;
    u16 assetCount;
    u32 tableOffset;
    u32 dataOffset;
} TNPackHeader;

typedef struct {
    char name[32];
    u16 type;
    u16 format;
    u32 offset;
    u32 size;
    u32 extraOffset;
} TNAssetEntry;

typedef struct {
    u32 sampleRate;
    u16 channels;
    u16 format;
    u32 totalSamples;
    u32 chunkSize;
    u32 chunkCount;
    u32 flags;
} TNAudioExtra;

#endif
