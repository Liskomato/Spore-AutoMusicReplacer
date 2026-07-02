#include "stdafx.h"
#include "RemoveReplacerMusic.h"
#include "AddReplacerMusic.h"
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
		RemoveActReplacementMusic(data);
		App::ConsolePrintF("Removed music replacement from act %d", data->GetEditModeActIndex() + 1);

	}
	else {
		App::ConsolePrintF("You have to be in the adventure editor to use this cheat.");
	}

}

int RemoveReplacerMusic::AddRef() { return DefaultRefCounted::AddRef(); }

int RemoveReplacerMusic::Release() { return DefaultRefCounted::Release(); }

int RemoveReplacerMusic::GetEventFlags() const { return UTFWin::kEventFlagBasicInput; }

void* RemoveReplacerMusic::Cast(uint32_t type) const {
	CLASS_CAST(RemoveReplacerMusic);

	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	return nullptr;
}

bool RemoveReplacerMusic::HandleUIMessage(UTFWin::IWindow* pWindow, const UTFWin::Message& message) {
	if (message.eventType != kMsgButtonClick)
		return false;
	if (pWindow->GetControlID() != CONTROL_ID_AMR_BUTTON_DELETE)
		return false;
	auto data = ScenarioMode.GetData();
	RemoveActReplacementMusic(data);
	return true;
}

void RemoveReplacerMusic::InitializeUI(IWindow* window, UILayout* layout) {
	container = layout->FindWindowByID(CONTROL_ID_AMR_BUTTON_CONTAINER);
	if (!container) return;

	if (container->GetParent() != window)
		window->AddWindow(container);

	deleteButton = container->FindWindowByID(CONTROL_ID_AMR_BUTTON_DELETE);
	if (deleteButton) {
		deleteButton->AddWinProc(this);
	}
}

void RemoveReplacerMusic::RemoveActReplacementMusic(Simulator::cScenarioData* data) {
	auto& res = data->mpResource;
	Simulator::cScenarioAct* act = &res->mActs[data->GetEditModeActIndex()];
	data->StartHistoryEntry();
	MSclient->Remove(act, MSR_REPLACING_MUSIC_ID);
	data->CommitHistoryEntry();
	Audio::PlayAudio(SND_SKINNING_REMOVE);
	if (addReplacerCheat)
	addReplacerCheat->UpdateUI(false);
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
