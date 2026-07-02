#pragma once

#include <Spore\BasicIncludes.h>

using namespace UTFWin;

class AddReplacerMusic 
	: public ArgScript::ICommand
	, public Sporepedia::IShopperListener
	, public UTFWin::IWinProc
	, public DefaultRefCounted
{
public:
#pragma region constants
	static const uint32_t SHP_ACT_MUSIC = id("ScenarioActMusicShopper");
#pragma endregion Asset Browser Shopper IDs

	IWindow* container;
	IWindow* emptyButton; // IButton
	IWindow* filledButton; // IButton
	SporeTooltipWinProc* tooltip;

	AddReplacerMusic();
	~AddReplacerMusic();

	// Cheat + Sporepedia request class overrides
	void ParseLine(const ArgScript::Line& line) override;
	void OnShopperAccept(const ResourceKey& selection) override;
	
	// IWinProc overrides
	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	int GetEventFlags() const override;
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

	void InitializeUI(IWindow* window, UILayout* layout);
	void UpdateUI(bool isFilled, string16 tooltipText = u"");
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

