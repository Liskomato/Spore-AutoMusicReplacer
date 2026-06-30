#include "stdafx.h"
#include "RemoveReplacerMusic.h"
#include "globals.h"
#include <MicroStorageAPI.h>

RemoveReplacerMusic::RemoveReplacerMusic()
{
}


RemoveReplacerMusic::~RemoveReplacerMusic()
{
}


void RemoveReplacerMusic::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	if (Simulator::GetGameModeID() == GameModeIDs::kScenarioMode
		&& ScenarioMode.GetMode() == App::cScenarioMode::Mode::EditMode) {
		auto data = ScenarioMode.GetData();
		auto res = ScenarioMode.GetResource();
		Simulator::cScenarioAct* act = &res->mActs[data->GetEditModeActIndex()];
		data->StartHistoryEntry();
		MSclient->Remove(act, MSR_REPLACING_MUSIC_ID);
		data->CommitHistoryEntry();
		Audio::PlayAudio(SND_SKINNING_REMOVE);
		App::ConsolePrintF("Removed music replacement from act %d", data->GetEditModeActIndex()+1);

	}
	else {
		App::ConsolePrintF("You have to be in the adventure editor to use this cheat.");
	}

}

const char* RemoveReplacerMusic::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Removes alternate act music from act.";
	}
	else {
		return "RemoveReplacerMusic: Removes the alternate act music added with Auto-Music Replacer from the current act.";
	}
}
