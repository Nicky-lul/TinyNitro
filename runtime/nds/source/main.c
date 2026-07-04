#include <nds.h>
#include <fat.h>
#include <stdio.h>

#include "asset_loader.h"
#include "audio_engine.h"

static void drawTestScene(void) {
    iprintf("\x1b[2J");
    iprintf("TinyNitro Runtime\n");
    iprintf("-----------------\n");
    iprintf("A: play pickup SFX\n");
    iprintf("B: play next ambience chunk\n");
}

int main(void) {
    consoleDemoInit();
    iprintf("TinyNitro booting...\n");

    if (!fatInitDefault()) {
        iprintf("FAT init failed.\n");
        iprintf("Use an emulator/cart setup with FAT support.\n");
        while (1) swiWaitForVBlank();
    }

    tnAudioInit();

    TNPack pack;
    if (!tnLoadPack(&pack, "/data/game.tnpak")) {
        iprintf("Could not load /data/game.tnpak\n");
        while (1) swiWaitForVBlank();
    }

    iprintf("Loaded pack: %d assets\n", pack.header.assetCount);
    iprintf("Press START.\n");

    bool streamReady = false;

    while (1) {
        scanKeys();
        int down = keysDown();

        if (down & KEY_START) drawTestScene();

        if (down & KEY_A) {
            if (!tnAudioPlayPcm8Sfx(&pack, "pickup")) iprintf("pickup failed\n");
        }

        if (down & KEY_B) {
            if (!streamReady) streamReady = tnAudioPrepareStream(&pack, "ambience");
            tnAudioUpdateStream();
        }

        swiWaitForVBlank();
    }

    tnClosePack(&pack);
    return 0;
}
