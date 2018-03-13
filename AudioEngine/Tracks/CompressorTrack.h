#pragma once
#include "AudioTrack.h"
#include "maximilian.h"
#include "Utils.h"

class CompressorTrack : public AudioTrack
{
private:
	maxiDyn _compressor;

	double _attackPeriod = 100.0;
	double _releasePeriod = 200.0;
	double _gainThreshold = 0.2;
	double _compressionRatio = 10.0;

public:
	void Init(BaseAudioEngine *engine, AudioTrack *parent)
	{
		AudioTrack::Init(engine, parent);

		_compressor.setAttack(_attackPeriod);
		_compressor.setRelease(_releasePeriod);
		_compressor.setThreshold(_gainThreshold);
		_compressor.setRatio(_compressionRatio);
	}

	void Process(double(&samples)[2])
	{
		double s;
		s = samples[0];
		samples[0] *= 1.0 - _gains[0];
		Utils::Mix(_gains[0] * _compressor.compress(s), samples[0]);

		s = samples[1];
		samples[1] *= 1.0 - _gains[0];
		Utils::Mix(_gains[0] * _compressor.compress(s), samples[1]);
	}
};

