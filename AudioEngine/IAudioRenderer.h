#pragma once


class IAudioRenderer
{
protected:
	class BaseAudioEngine *_engine = NULL;
	
public:
	IAudioRenderer() = default;
	virtual ~IAudioRenderer() = default;

	virtual bool Init(BaseAudioEngine *engine) = 0;
	virtual bool GetFormat(WORD &numChannels, DWORD &sampleRate, WORD &bitsPerSample) = 0;
	virtual bool GetBuffer(BYTE *&buffer, UINT32 &numFrames) = 0;
	virtual bool ReleaseBuffer(UINT32 numFrames, DWORD flags) = 0;
};

