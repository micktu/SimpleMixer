#pragma once


class IAudioRenderer
{
protected:
	class IAudioEngine *_engine = NULL;
	
public:
	IAudioRenderer() {}
	virtual ~IAudioRenderer() = default;

	virtual bool Init(IAudioEngine *engine) = 0;
	virtual bool Render() = 0;
	virtual bool GetFormat(WORD &numChannels, DWORD &sampleRate, WORD &bitsPerSample) = 0;
	virtual bool GetBuffer(BYTE *&buffer, UINT32 &numFrames) = 0;
	virtual bool ReleaseBuffer(UINT32 numFrames, DWORD flags) = 0;
};

