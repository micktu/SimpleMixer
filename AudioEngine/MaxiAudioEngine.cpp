#include "MaxiAudioEngine.h"

MaxiAudioEngine::MaxiAudioEngine()
{
}


MaxiAudioEngine::~MaxiAudioEngine()
{
}

bool MaxiAudioEngine::Init(IAudioRenderer *renderer)
{
	__super::Init(renderer);
	maxiSettings::setup(_sampleRate, _numChannels, _sampleRate);

	return true;
}

bool MaxiAudioEngine::Update()
{
	BYTE *buffer = nullptr;
	UINT32 numSamples = 0;

	_renderer->GetBuffer(buffer, numSamples);

	float* floatBuf = reinterpret_cast<float*>(buffer);

	for (UINT32 i = 0; i < numSamples; i++)
	{
		float value = (float)_sine.sinewave(440.0);

		floatBuf[i * 2] = value;
		floatBuf[i * 2 + 1] = value;
	}

	_renderer->ReleaseBuffer(numSamples, 0);
	return false;
}
