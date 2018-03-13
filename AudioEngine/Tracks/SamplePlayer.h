#pragma once

#include "common.h"
#include "AudioTrack.h"
#include "maximilian.h"

class SamplePlayer : public AudioTrack
{
private:
	maxiSample _sample;
	CHAR _fileName[MAX_PATH];

public:
	void Process(double(&samples)[2])
	{
		if (_sample.length < 1) return;

		samples[0] = _sample.play();
		samples[1] = samples[0];

		AudioTrack::Process(samples);
	}

	void SetFile(CHAR* fileName)
	{
		strcpy_s(_fileName, MAX_PATH, fileName);
		Utils::FullPathToFileA(_fileName);
		_sample.loadOgg(_fileName, 0);
	}
};

