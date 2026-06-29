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
		request.shopperID = id("ScenarioActMusicShopper");
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

		if (App::Property::GetUInt32(propListOld.get(), 0xb6878619,adventureMusicId)) {

			
			PropertyListPtr propList = new App::PropertyList();

			IO::SharedPointer* sharedPtr = new IO::SharedPointer(0, nullptr);
			MemoryStreamPtr memoryStream = new IO::MemoryStream(sharedPtr, 0);
			memoryStream->SetData(sharedPtr, 0);
			memoryStream->SetOption(IO::MemoryStream::kOptionResizeEnabled, 1);
			

			propList->SetProperty(0xb6878619, &App::Property().SetValueUInt32(adventureMusicId));


			if (propList->Write(memoryStream.get())) {
				auto scnres = ScenarioMode.GetResource();
				auto data = ScenarioMode.GetData();
				Simulator::cScenarioAct* currentAct = &scnres->mActs[data->GetEditModeActIndex()];
				data->StartHistoryEntry();
				MSclient->Write(memoryStream.get(),id("AMR-ReplacingMusicId"),currentAct);
				data->CommitHistoryEntry();
				App::ConsolePrintF("Stored alternate music ID %#010x to act %d",adventureMusicId,data->GetEditModeActIndex() + 1);
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
