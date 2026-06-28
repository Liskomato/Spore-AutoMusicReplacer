#include "stdafx.h"
#include "globals.h"
#include "RemoveReplacerMusic.h"

RemoveReplacerMusic::RemoveReplacerMusic()
{
}


RemoveReplacerMusic::~RemoveReplacerMusic()
{
}


void RemoveReplacerMusic::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
}

const char* RemoveReplacerMusic::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "RemoveReplacerMusic: Elaborate description of what this cheat does.";
	}
}
