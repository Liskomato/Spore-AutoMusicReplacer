#pragma once

#include <Spore\BasicIncludes.h>

#define AddReplacerCastAudioPtr intrusive_ptr<AddReplacerCastAudio>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;

class AddReplacerCastAudio 
	: public IWinProc
	, public DefaultRefCounted
	, public Sporepedia::IShopperListener
{
public:
	static const uint32_t TYPE = id("AddReplacerCastAudio");
#pragma region constants
	static const uint32_t SHP_FIXEDOBJECTAUDIO_AMR = id("ScenarioFixedObjectAudioShopper_AutoMusicReplacer");
#pragma endregion Asset Browser Shopper IDs
	AddReplacerCastAudio();
	~AddReplacerCastAudio();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;
	void OnShopperAccept(const ResourceKey& selection) override;
	void InitializeUI(IWindow* window,UILayout* layout, Simulator::cScenarioClass* target, int index);
	void UpdateUI(bool isFilled, string16 tooltipText = u"");
private:
	IWindow* container = nullptr;
	IWindow* emptyButton = nullptr; // IButton
	IWindow* filledButton = nullptr; // IButton
	SporeTooltipWinProc* tooltip = nullptr;
	
	Simulator::cScenarioClass* target = nullptr;
	int targetIndex = -1;
};
