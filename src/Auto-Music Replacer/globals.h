#pragma once

class MicroStorageClient;
class AddReplacerMusic;
class RemoveReplacerMusic;
class AddReplacerCastAudio;
class RemoveReplacerCastAudio;

extern MicroStorageClient* MSclient;

extern AddReplacerMusic* addReplacerCheat;

extern RemoveReplacerMusic* removeReplacerCheat;

extern AddReplacerCastAudio* addReplacerCastAudio;
extern RemoveReplacerCastAudio* removeReplacerCastAudio;

extern UILayoutPtr layoutBehaviour;
extern UILayoutPtr layoutActBar;

extern bool active;
#pragma region constants
static const char*    MSC_MOD_ID = "AutoMusicReplacer";
static const uint32_t MSR_REPLACING_MUSIC_ID = id("ReplacingMusicId");
#pragma endregion MicroStorage IDs

#pragma region constants
static const uint32_t PRP_ADVENTURE_MUSIC_ID = id("soundId");
static const uint32_t PRP_MUSIC_REPLACER_ACTIVE = id("MusicReplacerActive");
static const uint32_t PRP_SPOREPEDIA_NAME = 0x071E9BD1;
#pragma endregion Property IDs

#pragma region constants
static const uint32_t SND_SKINNING_REMOVE = id("editor_trash");
#pragma endregion Sound IDs

#pragma region constants
static const uint32_t MSG_UPDATE_CAST_PALETTE = 0x795b639;
static const uint32_t MSG_SHOW_BEHAVIOR_EDIT_UI = 0x7465518;
static const uint32_t MSG_HIDE_BEHAVIOR_EDIT_UI = 0x7d11fe0;
#pragma endregion Messsage IDs

#pragma region constants
static const uint32_t CONTROL_ID_AMR_BUTTON_CONTAINER = id("AMR-ActMusicReplacer-Container");
static const uint32_t CONTROL_ID_AMR_BUTTON_EMPTY = id("AMR-ActMusicReplacer-Empty");
static const uint32_t CONTROL_ID_AMR_BUTTON_FILLED = id("AMR-ActMusicReplacer-Filled");
static const uint32_t CONTROL_ID_AMR_BUTTON_DELETE = id("AMR-ActMusicReplacer-Delete");
static const uint32_t CONTROL_ID_ACTBAR_DESCRIPTION_AND_MUSIC = 0x075F60B0;
static const uint32_t CONTROL_ID_SCENARIO_EDIT_MODE_BEHAVIOR_CHATTER_AUDIO = 0x083CA530;
static const uint32_t CONTROL_ID_CAST_PREVIEW = 0x076C61C8;
#pragma endregion Control IDs

#pragma region constants
static const uint32_t SPUI_AMR_ACTMUSICBUTTON = id("AMR-ActMusicButton");
#pragma endregion SPUI IDs