#include "stdafx.h"
#include "MusicReplacerEnabled.h"
#include "globals.h"
#include "stdio.h"
#include "shlobj.h"
#include <string>


MusicReplacerEnabled::MusicReplacerEnabled()
{
	if (LoadData()) {
		App::ConsolePrintF("Auto-Music Replacer: Loaded preference data. Current state: %s", active ? "on" : "off");
	}
}


MusicReplacerEnabled::~MusicReplacerEnabled()
{
}


void MusicReplacerEnabled::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	if (Simulator::GetGameModeID() == GameModeIDs::kScenarioMode && ScenarioMode.GetMode() == App::cScenarioMode::Mode::PlayMode) {
		App::ConsolePrintF("You cannot use this cheat in adventure play mode.");
		return;
	}

	auto args = line.GetArguments(1);
	active = mpFormatParser->ParseBool(args[0]);
	if (active) {
		App::ConsolePrintF("Auto-Music Replacer has been enabled.");
	}
	else {
		App::ConsolePrintF("Auto-Music Replacer has been disabled.");
	}
	if (!SaveData()) {
		App::ConsolePrintF("Saving Auto-Music Replacer status has failed.");
	}

}

const char* MusicReplacerEnabled::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Enables/disables Auto-Music Replacer.";
	}
	else {
		return "MusicReplacerEnabled: Enables or disables Auto-Music Replacer; meaning you can play adventures with either their original or Auto-Music Replacer audio.";
	}
}

string16 MusicReplacerEnabled::GetFilePath() {
	// Get %APPDATA%
	PWSTR appdata;
	/*	size_t len;
		errno_t err = getenv_s(&appdata, &len, u"APPDATA");*/

	if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &appdata) != S_OK) {
		// This shouldn't happen.
		return NULL;
	}


	// Add rest of the filepath.

	string16 path = (char16_t*)appdata;

	path += u"\\Spore\\Preferences\\MusicReplacerStatus.prop";

	return path;
}

bool MusicReplacerEnabled::SaveData() {
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilePath().c_str());

	propList->SetProperty(PRP_MUSIC_REPLACER_ACTIVE, &App::Property().SetValueBool(active));
	
	stream->Open(IO::AccessFlags::ReadWrite, IO::CD::CreateAlways);
	bool check = propList->Write(stream.get());
	stream->Close();
	return check;

}

bool MusicReplacerEnabled::LoadData() {
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilePath().c_str());
	if (stream->Open(IO::AccessFlags::Read, IO::CD::OpenExisting)) {
		propList->Read(stream.get());
		stream->Close();
	}

	if (propList->HasProperty(PRP_MUSIC_REPLACER_ACTIVE)) {
		App::Property::GetBool(propList.get(), PRP_MUSIC_REPLACER_ACTIVE, active);
		return true;
	}
	else {
		bool check = SaveData();
		if (check) App::ConsolePrintF("Auto-Music Replacer: Settings data didn't exist, so it was created to %ls", GetFilePath().c_str());
		else App::ConsolePrintF("Auto-Music Replacer: SaveData failed! Settings file will be attempted to be created to %ls once the game is closed.", GetFilePath().c_str());
		return check;
	}
}