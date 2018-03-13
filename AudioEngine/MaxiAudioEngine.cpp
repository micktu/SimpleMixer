#include "MaxiAudioEngine.h"
#include "SineGenerator.h"

MaxiAudioEngine::MaxiAudioEngine()
{
}


MaxiAudioEngine::~MaxiAudioEngine()
{
}

bool MaxiAudioEngine::Init(IAudioRenderer *renderer)
{
	BaseAudioEngine::Init(renderer);

	maxiSettings::setup(_sampleRate, _numChannels, 1024);

	return true;
}

