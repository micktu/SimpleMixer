#pragma once

#include "BaseAudioEngine.h"


class MaxiAudioEngine :	public BaseAudioEngine
{
private:
	class SineGenerator *gen;

public:
	MaxiAudioEngine();
	~MaxiAudioEngine();

	bool Init(class IAudioRenderer *renderer) override;
};

