#include "AudioTrack.h"

void AudioTrack::Init(BaseAudioEngine * engine, AudioTrack * parent)
{
	_engine = engine;
	_parent = parent;
}

void AudioTrack::Process(double(&samples)[2])
{
	for (size_t i = 0; i < _numChildren; i++)
	{
		_children[i]->Process(samples);
	}
}
