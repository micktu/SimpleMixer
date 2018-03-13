#pragma once

#include "AudioTrack.h"
#include "maximilian.h"
#include "Utils.h"

class SineGenerator : public AudioTrack
{
public:
	double Frequency = 440.0;

private:
	maxiOsc _osc;

public:
	void Process(double (&samples)[2]) override
	{
		double value = _osc.sinewave(Frequency);
		double values[2] = { value, value };
		Utils::Mix(values, samples);
		AudioTrack::Process(samples);
	}

};

