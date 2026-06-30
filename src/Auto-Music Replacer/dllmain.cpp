// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MusicReplacerEnabled.h"
#include "AddReplacerMusic.h"
#include "RemoveReplacerMusic.h"
#include "globals.h"
#include <MicroStorageAPI.h>


MicroStorageClient* MSclient = nullptr;
AddReplacerMusic* addReplacerCheat = nullptr;
RemoveReplacerMusic* removeReplacerCheat = nullptr;
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

	MSclient = new MicroStorageClient("AutoMusicReplacer");
	addReplacerCheat = new AddReplacerMusic();
	removeReplacerCheat = new RemoveReplacerMusic();
	CheatManager.AddCheat("musicreplacer",new MusicReplacerEnabled());
	CheatManager.AddCheat("addreplacermusic", addReplacerCheat);
	CheatManager.AddCheat("removereplacermusic", removeReplacerCheat);
}

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

				IO::SharedPointer* sharedPtr = new IO::SharedPointer(0, nullptr);
				MemoryStreamPtr memoryStream = new IO::MemoryStream(sharedPtr, 0);
				memoryStream->SetData(sharedPtr, 0);
				memoryStream->SetOption(IO::MemoryStream::kOptionResizeEnabled, 1);

				if (MSclient->Read(actP, id("AMR-ReplacingMusicId"), memoryStream.get())) {
					PropertyListPtr propList = new App::PropertyList();
					propList->Read(memoryStream.get());
					if (propList->HasProperty(0xb6878619)) {
						App::Property::GetUInt32(propList.get(), 0xb6878619, alternateMusicId);
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
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));

	cScenarioPlayMode_Initialize_dtr::attach(GetAddress(Simulator::cScenarioPlayMode, Initialize));
	cScenarioPlayMode_SetCurrentAct_dtr::attach(GetAddress(Simulator::cScenarioPlayMode, SetCurrentAct));
	AudioSystem_PlayActMusic_dtr::attach(Address(ModAPI::ChooseAddress(0x657050,0x657200)));

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

