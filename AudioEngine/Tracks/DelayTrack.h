#pragma once
#include "AudioTrack.h"
#include "maximilian.h"


class DelayTrack : public AudioTrack
{
private:
	maxiDelayline _delay;

public:
	void Init(BaseAudioEngine *engine, AudioTrack *parent) override
	{
		AudioTrack::Init(engine, parent);
	}

	void Process(double(&samples)[2]) override
	{
		samples[0] = _delay.dl(samples[0], 24000, 0.5);
		samples[1] = samples[0];
	}
};

