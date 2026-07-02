#pragma once

class MicroStorageClient;
class AddReplacerMusic;
class RemoveReplacerMusic;
class AddReplacerCastAudio;
class RemoveReplacerCastAudio;

extern MicroStorageClient* MSclient;

extern AddReplacerMusic* addReplacerCheat;

extern RemoveReplacerMusic* removeReplacerCheat;

extern UILayoutPtr layoutBehaviour;
extern UILayoutPtr layoutActBar;

extern AddReplacerCastAudioPtr addReplacerCastAudio;
extern RemoveReplacerCastAudioPtr removeReplacerCastAudio;

extern bool active;

#pragma region constants
static const char*    MSC_MOD_ID = "AutoMusicReplacer";
static const uint32_t MSR_REPLACING_MUSIC_ID = id("ReplacingMusicId");
#pragma endregion MicroStorage IDs

#pragma region constants
static const uint32_t PRP_ADVENTURE_MUSIC_ID = id("soundId");
static const uint32_t PRP_MUSIC_REPLACER_ACTIVE = id("MusicReplacerActive");
#pragma endregion Property IDs
