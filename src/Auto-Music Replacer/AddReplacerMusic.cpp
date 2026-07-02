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

int AddReplacerMusic::AddRef() { return DefaultRefCounted::AddRef(); }

int AddReplacerMusic::Release() { return DefaultRefCounted::Release(); }

int AddReplacerMusic::GetEventFlags() const { return UTFWin::kEventFlagBasicInput; }

void* AddReplacerMusic::Cast(uint32_t type) const {
	CLASS_CAST(AddReplacerMusic);

	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	return nullptr;
}

bool AddReplacerMusic::HandleUIMessage(UTFWin::IWindow* pWindow, const UTFWin::Message& message) {
	if (message.eventType != kMsgButtonClick) {
		return false;
	}
	Sporepedia::ShopperRequest request(this);
	request.shopperID = SHP_ACT_MUSIC;
	switch (pWindow->GetControlID()) {
	case CONTROL_ID_AMR_BUTTON_EMPTY:
	case CONTROL_ID_AMR_BUTTON_FILLED:
		Sporepedia::ShopperRequest::Show(request);
		return true;
	default:
		return false;
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
		LocalizedString name;

		if (App::Property::GetUInt32(propListOld.get(), PRP_ADVENTURE_MUSIC_ID, adventureMusicId) && App::Property::GetText(propListOld.get(),PROP_SPOREPEDIA_NAME,name)) {

			
			PropertyListPtr propList = new App::PropertyList();

			MemoryStreamPtr memoryStream = new IO::MemoryStream(nullptr, 0);
			memoryStream->SetOption(IO::MemoryStream::kOptionResizeEnabled, 1);
			

			propList->SetProperty(PRP_ADVENTURE_MUSIC_ID, &App::Property().SetValueUInt32(adventureMusicId));
			propList->SetProperty(PROP_SPOREPEDIA_NAME, &App::Property().SetValueString16(name.GetText()));

			if (propList->Write(memoryStream.get())) {
				auto scnres = ScenarioMode.GetResource();
				auto data = ScenarioMode.GetData();
				Simulator::cScenarioAct* currentAct = &scnres->mActs[data->GetEditModeActIndex()];
				data->StartHistoryEntry();
				MSclient->Write(memoryStream.get(), MSR_REPLACING_MUSIC_ID, currentAct);
				data->CommitHistoryEntry();
				App::ConsolePrintF("Stored alternate music ID %#010x to act %d", adventureMusicId, data->GetEditModeActIndex() + 1);
				UpdateUI(true,name.GetText());
			}
		}
	}
	
}

void AddReplacerMusic::InitializeUI(IWindow* window, UILayout* layout) {
	container = layout->FindWindowByID(CONTROL_ID_AMR_BUTTON_CONTAINER);
	if (!container) return;

	if (container->GetParent() != window)
	window->AddWindow(container);

	emptyButton = container->FindWindowByID(CONTROL_ID_AMR_BUTTON_EMPTY);
	if (!emptyButton) {
		return;
	}
	emptyButton->AddWinProc(this);
	filledButton = container->FindWindowByID(CONTROL_ID_AMR_BUTTON_FILLED);
	if (filledButton) {
		filledButton->AddWinProc(this);
		IWinProc* tooltip1 = filledButton->GetNextWinProc();
		if (tooltip1 != nullptr) {
			IWinProc* tooltip2 = filledButton->GetNextWinProc(tooltip1);
			if (tooltip2 != nullptr && tooltip2->TYPE == SporeTooltipWinProc::TYPE)
			tooltip = object_cast<SporeTooltipWinProc>(tooltip1);
		}
	}

}

void AddReplacerMusic::UpdateUI(bool isFilled, string16 tooltipText = u"") {
	if (emptyButton && filledButton) {
		emptyButton->SetVisible(!isFilled);
		filledButton->SetVisible(isFilled);
		if (tooltip != nullptr) {
			tooltip->mText = tooltipText;
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
