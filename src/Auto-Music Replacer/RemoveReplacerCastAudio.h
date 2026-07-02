#pragma once

#include <Spore\BasicIncludes.h>

#define RemoveReplacerCastAudioPtr intrusive_ptr<RemoveReplacerCastAudio>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;

class RemoveReplacerCastAudio 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("RemoveReplacerCastAudio");
	
	RemoveReplacerCastAudio();
	~RemoveReplacerCastAudio();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;
	void InitializeUI(IWindow* window,UILayout* layout, Simulator::cScenarioClass* target, int index);
	
private:
	IWindow* container = nullptr;
	IWindow* deleteButton = nullptr; //IButton

	Simulator::cScenarioClass* target = nullptr;
	int targetIndex = -1;
};
