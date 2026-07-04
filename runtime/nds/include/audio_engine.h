#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <nds.h>
#include "asset_loader.h"

void tnAudioInit(void);
bool tnAudioPlayPcm8Sfx(TNPack* pack, const char* name);
bool tnAudioPrepareStream(TNPack* pack, const char* name);
void tnAudioUpdateStream(void);

#endif
