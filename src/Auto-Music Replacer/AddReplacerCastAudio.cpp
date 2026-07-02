#include "stdafx.h"
#include "globals.h"
#include "AddReplacerCastAudio.h"

AddReplacerCastAudio::AddReplacerCastAudio()
{
}


AddReplacerCastAudio::~AddReplacerCastAudio()
{
}

// For internal use, do not modify.
int AddReplacerCastAudio::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AddReplacerCastAudio::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* AddReplacerCastAudio::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(AddReplacerCastAudio);
	return nullptr;
}

// This method returns a combinations of values in UTFWin::EventFlags.
// The combination determines what types of events (messages) this window procedure receives.
// By default, it receives mouse/keyboard input and advanced messages.
int AddReplacerCastAudio::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

// The method that receives the message. The first thing you should do is probably
// checking what kind of message was sent...
bool AddReplacerCastAudio::HandleUIMessage(IWindow* window, const Message& message)
{
	if (message.eventType != kMsgButtonClick || !target) {
		return false;
	}
	Sporepedia::ShopperRequest request(this);
	request.shopperID = SHP_FIXEDOBJECTAUDIO_AMR;
	switch (window->GetControlID()) {
		case CONTROL_ID_AMR_BUTTON_EMPTY:
		case CONTROL_ID_AMR_BUTTON_FILLED:
			Sporepedia::ShopperRequest::Show(request);
			return true;
		default:
			return false;
	}
	// Return true if the message was handled, and therefore no other window procedure should receive it.
	return false;
}

void AddReplacerCastAudio::OnShopperAccept(const ResourceKey& selection) {

}

void AddReplacerCastAudio::InitializeUI(IWindow* window, UILayout* layout,Simulator::cScenarioClass* target, int index) {

}

void AddReplacerCastAudio::UpdateUI(bool isFilled, string16 tooltipText = u"") {

}