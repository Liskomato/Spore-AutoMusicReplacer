#pragma once

#include <Spore\BasicIncludes.h>

class AddReplacerMusic 
	: public ArgScript::ICommand,
	  Sporepedia::IShopperListener
{
public:
#pragma region constants
	static const uint32_t SHP_ACT_MUSIC = id("ScenarioActMusicShopper");
#pragma endregion Asset Browser Shopper IDs

	AddReplacerMusic();
	~AddReplacerMusic();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	void OnShopperAccept(const ResourceKey& selection) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

