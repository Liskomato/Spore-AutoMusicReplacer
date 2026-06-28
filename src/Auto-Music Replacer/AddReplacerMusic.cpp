#include "stdafx.h"
#include "globals.h"
#include "AddReplacerMusic.h"

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
}

const char* AddReplacerMusic::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "AddReplacerMusic: Elaborate description of what this cheat does.";
	}
}
