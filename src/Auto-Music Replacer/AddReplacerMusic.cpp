#include "stdafx.h"
#include "AddReplacerMusic.h"
#include "globals.h"
#include <MicroStorageAPI.h>

AddReplacerMusic::AddReplacerMusic()
{
}


AddReplacerMusic::~AddReplacerMusic()
{
}


void AddReplacerMusic::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	if (Simulator::GetGameModeID() == GameModeIDs::kScenarioMode
		&& ScenarioMode.GetMode() == App::cScenarioMode::Mode::EditMode) {
		Sporepedia::ShopperRequest request(this);
		request.shopperID = SHP_ACT_MUSIC;
		Sporepedia::ShopperRequest::Show(request);
	}
	else {
		App::ConsolePrintF("You have to be in the adventure editor to use this cheat.");
	}
}

void AddReplacerMusic::OnShopperAccept(const ResourceKey& selection) {

	
	Resource::Database* db = ResourceManager.FindRecord(selection, nullptr);
	if (db) {

		Resource::IRecord* record = nullptr;
		db->OpenRecord(selection,&record);

		IO::IStream* stream = record->GetStream();
		
		PropertyListPtr propListOld = new App::PropertyList();
		propListOld->Read(stream);

		uint32_t adventureMusicId;

		if (App::Property::GetUInt32(propListOld.get(), PRP_ADVENTURE_MUSIC_ID, adventureMusicId)) {

			
			PropertyListPtr propList = new App::PropertyList();

			MemoryStreamPtr memoryStream = new IO::MemoryStream(nullptr, 0);
			memoryStream->SetOption(IO::MemoryStream::kOptionResizeEnabled, 1);
			

			propList->SetProperty(PRP_ADVENTURE_MUSIC_ID, &App::Property().SetValueUInt32(adventureMusicId));


			if (propList->Write(memoryStream.get())) {
				auto scnres = ScenarioMode.GetResource();
				auto data = ScenarioMode.GetData();
				Simulator::cScenarioAct* currentAct = &scnres->mActs[data->GetEditModeActIndex()];
				data->StartHistoryEntry();
				MSclient->Write(memoryStream.get(), MSR_REPLACING_MUSIC_ID, currentAct);
				data->CommitHistoryEntry();
				App::ConsolePrintF("Stored alternate music ID %#010x to act %d", adventureMusicId, data->GetEditModeActIndex() + 1);
			}
		}
	}
	
}

const char* AddReplacerMusic::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Add alternate adventure act music for AMR users.";
	}
	else {
		return "AddReplacerMusic: Adds an alternate music track to adventure acts for other users of Auto-Music Replacer.";
	}
}
