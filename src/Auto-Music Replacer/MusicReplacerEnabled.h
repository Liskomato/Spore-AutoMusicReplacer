#pragma once

#include <Spore\BasicIncludes.h>

class MusicReplacerEnabled 
	: public ArgScript::ICommand
{
public:
	MusicReplacerEnabled();
	~MusicReplacerEnabled();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	string16 GetFilePath();
	bool SaveData();
	bool LoadData();
};

