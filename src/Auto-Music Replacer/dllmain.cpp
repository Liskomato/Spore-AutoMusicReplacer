// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "globals.h"
#include <MicroStorageAPI.h>
#include "MusicReplacerEnabled.h"
#include "AddReplacerMusic.h"
#include "RemoveReplacerMusic.h"

MicroStorageClient* client = nullptr;

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials

	client = new MicroStorageClient("AutoMusicReplacer");
	CheatManager.AddCheat("musicreplacer",new MusicReplacerEnabled());
	CheatManager.AddCheat("addreplacermusic", new AddReplacerMusic());
	CheatManager.AddCheat("removereplacermusic", new RemoveReplacerMusic());
}



static_detour(AudioSystem_PlayAudio_dtr, void(uint32_t, Audio::AudioTrack)) {

	void detoured(uint32_t soundID, Audio::AudioTrack track) {
		if (Simulator::GetGameModeID() == GameModeIDs::kScenarioMode
			&& ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode
			&& soundID == ScenarioMode.GetResource()->mActs[ScenarioMode.GetPlayMode()->mCurrentActIndex].mActMusicID
			&& track == ScenarioMode.GetPlayMode()->mMusicTrack
			&& active) 
		{


		}
		else {
			original_function(soundID, track);
		}
	}
};

void Dispose()
{
	// This method is called when the game is closing
	if (client) {
		delete client;
		client = nullptr;
	}
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));


	AudioSystem_PlayAudio_dtr::attach(GetAddress(Audio,PlayAudio));

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

