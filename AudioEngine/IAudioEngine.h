#pragma once

#include "IAudioRenderer.h"

template <class TEngine, class TRenderer>
TEngine* InitAudioEngine()
{
	TEngine *engine = new TEngine();
	engine->Init(new TRenderer());
	return engine;
}

class IAudioEngine
{
protected:
	IAudioRenderer *_renderer = NULL;

	WORD _numChannels = 0;
	DWORD _sampleRate = 0;
	WORD _bitsPerSample = 0;

public:
	virtual ~IAudioEngine()
	{
		delete _renderer;
	}

	virtual bool Init(IAudioRenderer *renderer)
	{
		_renderer = renderer;
		_renderer->Init(this);
		_renderer->GetFormat(_numChannels, _sampleRate, _bitsPerSample);

		return true;
	}

	virtual bool Update() = 0;
};

