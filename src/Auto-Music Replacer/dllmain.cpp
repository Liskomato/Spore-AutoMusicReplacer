// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MusicReplacerEnabled.h"
#include "AddReplacerMusic.h"
#include "RemoveReplacerMusic.h"
#include "AddReplacerCastAudio.h"
#include "RemoveReplacerCastAudio.h"
#include "globals.h"
#include <MicroStorageAPI.h>

class cScenarioEditModeScriptUI {};
class ActEditorBar {};

UILayoutPtr layoutBehaviour = nullptr;
UILayoutPtr layoutActBar = nullptr;

MicroStorageClient* MSclient = nullptr;
AddReplacerMusic* addReplacerCheat = nullptr;
RemoveReplacerMusic* removeReplacerCheat = nullptr;
AddReplacerCastAudioPtr addReplacerCastAudio = nullptr;
RemoveReplacerCastAudioPtr removeReplacerCastAudio = nullptr;
eastl::map<uint32_t, uint32_t> alternateMusicIDs; // first uint32 is for act index, second for actual music ID

bool active = true;

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials

	MSclient = new MicroStorageClient(MSC_MOD_ID);
	addReplacerCheat = new AddReplacerMusic();
	removeReplacerCheat = new RemoveReplacerMusic();
	CheatManager.AddCheat("musicreplacer",new MusicReplacerEnabled());
	CheatManager.AddCheat("addreplacermusic", addReplacerCheat);
	CheatManager.AddCheat("removereplacermusic", removeReplacerCheat);
}

member_detour(cScenarioEditModeScriptUI_ShowBehaviorEditUI, cScenarioEditModeScriptUI, void())
{
	void detoured() {
		original_function(this);

		uintptr_t thisPtr = (uintptr_t)this;
		cScenarioDataPtr scenarioData = ScenarioMode.mpData;
		if (!scenarioData)
			return;

		uint32_t index = (uint32_t)field(thisPtr, 0x20);
		Simulator::cScenarioClass* scenarioClass = scenarioData->GetClass(index);
		if (!scenarioClass
			|| Simulator::cScenarioClass::GetObjectType(*(ResourceKey*)scenarioClass) !=
			Simulator::ScenarioObjectType::ScenarioFixedObjectAudio)
			return;

		UTFWin::IWindow* behaviorEditUIWin = (UTFWin::IWindow*)field(thisPtr, 0x18);
		if (!behaviorEditUIWin)
			return;

	}
};

member_detour(ActEditorBar_load, ActEditorBar, uint32_t* (int*)) {
	uint32_t* detoured(int* p1) {
		uint32_t* r = original_function(this, p1);
		return r;
	}
};

member_detour(cScenarioData_UpdateEditorAct, Simulator::cScenarioData, void(int, Simulator::cScenarioMarker*, int)) {
	void detoured(int i1, Simulator::cScenarioMarker * markers, int i2) {
		original_function(this, i1, markers, i2);
	}
};

member_detour(cScenarioPlayMode_Initialize_dtr, Simulator::cScenarioPlayMode, void()) {

	void detoured() {
		original_function(this);
		// Skip the entire function if the mod isn't active.
		if (active) {

			alternateMusicIDs.clear(); // Cleaning for the next adventure run.

			auto scnRes = ScenarioMode.GetResource();
			uint32_t i = 0;  // iterator; act 1 is 0.
			uint32_t alternateMusicId;

			for (Simulator::cScenarioAct act : scnRes->mActs) {

				Simulator::cScenarioAct* actP = &act;

				MemoryStreamPtr memoryStream = new IO::MemoryStream(nullptr, 0);
				memoryStream->SetOption(IO::MemoryStream::kOptionResizeEnabled, 1);

				if (MSclient->Read(actP, MSR_REPLACING_MUSIC_ID, memoryStream.get())) {
					PropertyListPtr propList = new App::PropertyList();
					propList->Read(memoryStream.get());
					if (propList->HasProperty(PRP_ADVENTURE_MUSIC_ID)) {
						App::Property::GetUInt32(propList.get(), PRP_ADVENTURE_MUSIC_ID, alternateMusicId);
						alternateMusicIDs.emplace(i, alternateMusicId);
					}
				}
				i++;
			}
		}
	}

};

member_detour(cScenarioPlayMode_SetCurrentAct_dtr, Simulator::cScenarioPlayMode, void(int, bool)) {

	void detoured(int actIndex, bool boolean) {
		original_function(this, actIndex, boolean);
	}

};

static_detour(AudioSystem_PlayActMusic_dtr, void(uint32_t, Audio::AudioTrack)) {

	void detoured(uint32_t soundID, Audio::AudioTrack track) {
		auto currentAct = ScenarioMode.GetPlayMode()->mCurrentActIndex;
		if (active && alternateMusicIDs.find(currentAct) != alternateMusicIDs.end())
		{
			original_function(alternateMusicIDs[currentAct], track);

		}
		else {
			original_function(soundID, track);
		}
	}
};


void Dispose()
{
	// This method is called when the game is closing
	if (MSclient) {
		delete MSclient;
		MSclient = nullptr;
	}
	if (addReplacerCheat) {
		delete addReplacerCheat;
		addReplacerCheat = nullptr;
	}
	if (removeReplacerCheat) {
		delete removeReplacerCheat;
		removeReplacerCheat = nullptr;
	}
	layoutBehaviour = nullptr;
	layoutActBar = nullptr;

	addReplacerCastAudio = nullptr;
	removeReplacerCastAudio = nullptr;
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));

	cScenarioPlayMode_Initialize_dtr::attach(GetAddress(Simulator::cScenarioPlayMode, Initialize));
	cScenarioPlayMode_SetCurrentAct_dtr::attach(GetAddress(Simulator::cScenarioPlayMode, SetCurrentAct));
	AudioSystem_PlayActMusic_dtr::attach(Address(ModAPI::ChooseAddress(0x657050,0x657200)));

	cScenarioEditModeScriptUI_ShowBehaviorEditUI::attach(Address(ModAPI::ChooseAddress(0xeb3030, 0xedee10)));
	ActEditorBar_load::attach(Address(ModAPI::ChooseAddress(0xedbbd0,0xedb9c0)));
	cScenarioData_UpdateEditorAct::attach(Address(ModAPI::ChooseAddress(0xf462a0,0xf462b0)));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

