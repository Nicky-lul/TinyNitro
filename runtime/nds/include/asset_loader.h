#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <nds.h>
#include <stdio.h>
#include "tinynitro_format.h"

#define TN_MAX_ASSETS 64

typedef struct {
    FILE* file;
    TNPackHeader header;
    TNAssetEntry assets[TN_MAX_ASSETS];
    int loaded;
} TNPack;

bool tnLoadPack(TNPack* pack, const char* path);
const TNAssetEntry* tnFindAsset(TNPack* pack, const char* name);
bool tnReadAsset(TNPack* pack, const TNAssetEntry* entry, void* dest);
bool tnReadAudioExtra(TNPack* pack, const TNAssetEntry* entry, TNAudioExtra* extra);
bool tnReadAt(TNPack* pack, u32 offset, void* dest, u32 size);
void tnClosePack(TNPack* pack);

#endif
