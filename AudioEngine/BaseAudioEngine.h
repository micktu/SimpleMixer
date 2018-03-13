#pragma once

#include "common.h"
#include "IAudioRenderer.h"

class BaseAudioEngine
{
protected:
	IAudioRenderer *_renderer = NULL;

	WORD _numChannels = 0;
	DWORD _sampleRate = 0;
	WORD _bitsPerSample = 0;

	std::vector<class AudioTrack *> _tracks;

public:
	BaseAudioEngine() = default;
	virtual ~BaseAudioEngine();
	
	virtual bool Init(IAudioRenderer *renderer);
	virtual bool Update();

	template <class TEngine, class TRenderer>
	static TEngine* BaseAudioEngine::InitAudioEngine();

	template <class TAudioTrack>
	TAudioTrack * AddTrack(AudioTrack *parent = nullptr);

	__forceinline std::vector<AudioTrack *> GetTracks() const { return _tracks; }
};

template <class TEngine, class TRenderer>
TEngine * BaseAudioEngine::InitAudioEngine()
{
	TEngine *engine = new TEngine();
	engine->Init(new TRenderer());
	return engine;
}

template <class TAudioTrack>
TAudioTrack * BaseAudioEngine::AddTrack(AudioTrack * parent)
{
	TAudioTrack *track = new TAudioTrack();
	track->Init(this, parent);
	_tracks.push_back(track);
	return track;
}
