#pragma once
#include "AudioTrack.h"
#include "maximilian.h"


class LowpassTrack :
	public AudioTrack
{
private:
	maxiFilter _filter;

public:
	double Cutoff = 1.0;

public:
	void Init(BaseAudioEngine *engine, AudioTrack *parent) override
	{
		AudioTrack::Init(engine, parent);
		_filter.resonance = 0.1;
	}

	void Process(double(&samples)[2]) override
	{
		samples[0] = _filter.lopass(samples[0], Cutoff);
		samples[1] = samples[0];
	}
};

