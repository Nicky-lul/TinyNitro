#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "audio_engine.h"

#define TN_STREAM_BUFFER_SIZE 32768

static TNPack* gStreamPack = NULL;
static TNAssetEntry gStreamEntry;
static TNAudioExtra gStreamExtra;
static u32 gStreamChunk = 0;
static u8* gStreamBuffer = NULL;

void tnAudioInit(void) {
    soundEnable();
    gStreamBuffer = (u8*)malloc(TN_STREAM_BUFFER_SIZE);
}

bool tnAudioPlayPcm8Sfx(TNPack* pack, const char* name) {
    const TNAssetEntry* entry = tnFindAsset(pack, name);
    if (!entry || entry->type != TN_ASSET_AUDIO || entry->format != TN_AUDIO_PCM8) return false;

    TNAudioExtra extra;
    if (!tnReadAudioExtra(pack, entry, &extra)) return false;

    u8* buffer = (u8*)malloc(entry->size);
    if (!buffer) return false;

    if (!tnReadAsset(pack, entry, buffer)) {
        free(buffer);
        return false;
    }

    soundPlaySample(
        buffer,
        SoundFormat_8Bit,
        entry->size,
        extra.sampleRate,
        127,
        64,
        false,
        0
    );

    return true;
}

bool tnAudioPrepareStream(TNPack* pack, const char* name) {
    const TNAssetEntry* entry = tnFindAsset(pack, name);
    if (!entry || entry->type != TN_ASSET_AUDIO || entry->format != TN_AUDIO_PCM8) return false;
    if (!tnReadAudioExtra(pack, entry, &gStreamExtra)) return false;

    if (!gStreamBuffer) {
        gStreamBuffer = (u8*)malloc(TN_STREAM_BUFFER_SIZE);
        if (!gStreamBuffer) return false;
    }

    gStreamPack = pack;
    memcpy(&gStreamEntry, entry, sizeof(TNAssetEntry));
    gStreamChunk = 0;
    return true;
}

void tnAudioUpdateStream(void) {
    if (!gStreamPack || !gStreamBuffer) return;

    u32 chunkSize = gStreamExtra.chunkSize;
    if (chunkSize == 0 || chunkSize > TN_STREAM_BUFFER_SIZE) chunkSize = TN_STREAM_BUFFER_SIZE;

    u32 packEnd = gStreamEntry.offset + gStreamEntry.size;
    u32 offset = gStreamEntry.offset + (gStreamChunk * chunkSize);

    if (offset >= packEnd) {
        if (gStreamExtra.flags & TN_AUDIO_FLAG_LOOP) {
            gStreamChunk = 0;
            offset = gStreamEntry.offset;
        } else {
            gStreamPack = NULL;
            return;
        }
    }

    u32 remaining = packEnd - offset;
    u32 readSize = remaining < chunkSize ? remaining : chunkSize;

    if (!tnReadAt(gStreamPack, offset, gStreamBuffer, readSize)) {
        gStreamPack = NULL;
        return;
    }

    soundPlaySample(
        gStreamBuffer,
        SoundFormat_8Bit,
        readSize,
        gStreamExtra.sampleRate,
        90,
        64,
        false,
        0
    );

    gStreamChunk++;
}
