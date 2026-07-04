#include <nds.h>
#include <stdio.h>
#include <string.h>
#include "asset_loader.h"

bool tnReadAt(TNPack* pack, u32 offset, void* dest, u32 size) {
    if (!pack || !pack->file) return false;
    if (fseek(pack->file, offset, SEEK_SET) != 0) return false;
    return fread(dest, 1, size, pack->file) == size;
}

bool tnLoadPack(TNPack* pack, const char* path) {
    memset(pack, 0, sizeof(TNPack));

    pack->file = fopen(path, "rb");
    if (!pack->file) return false;

    if (fread(&pack->header, 1, sizeof(TNPackHeader), pack->file) != sizeof(TNPackHeader)) {
        tnClosePack(pack);
        return false;
    }

    if (memcmp(pack->header.magic, "TNPK", 4) != 0) {
        tnClosePack(pack);
        return false;
    }

    if (pack->header.version != TN_VERSION) {
        tnClosePack(pack);
        return false;
    }

    if (pack->header.assetCount > TN_MAX_ASSETS) {
        tnClosePack(pack);
        return false;
    }

    if (fseek(pack->file, pack->header.tableOffset, SEEK_SET) != 0) {
        tnClosePack(pack);
        return false;
    }

    size_t count = fread(pack->assets, sizeof(TNAssetEntry), pack->header.assetCount, pack->file);
    if (count != pack->header.assetCount) {
        tnClosePack(pack);
        return false;
    }

    pack->loaded = 1;
    return true;
}

const TNAssetEntry* tnFindAsset(TNPack* pack, const char* name) {
    if (!pack || !pack->loaded) return NULL;

    for (int i = 0; i < pack->header.assetCount; i++) {
        if (strncmp(pack->assets[i].name, name, 32) == 0) {
            return &pack->assets[i];
        }
    }

    return NULL;
}

bool tnReadAsset(TNPack* pack, const TNAssetEntry* entry, void* dest) {
    if (!entry) return false;
    return tnReadAt(pack, entry->offset, dest, entry->size);
}

bool tnReadAudioExtra(TNPack* pack, const TNAssetEntry* entry, TNAudioExtra* extra) {
    if (!entry || !extra || entry->extraOffset == 0) return false;
    return tnReadAt(pack, entry->extraOffset, extra, sizeof(TNAudioExtra));
}

void tnClosePack(TNPack* pack) {
    if (pack && pack->file) fclose(pack->file);
    if (pack) memset(pack, 0, sizeof(TNPack));
}
