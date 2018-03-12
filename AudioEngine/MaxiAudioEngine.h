#pragma once

#include "IAudioEngine.h"
#include "maximilian.h"

class MaxiAudioEngine :	public IAudioEngine
{
private:
	maxiOsc _sine;

public:
	MaxiAudioEngine();
	~MaxiAudioEngine();

	bool Init(class IAudioRenderer *renderer) override;
	bool Update() override;
};

