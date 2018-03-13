#pragma once

#include <AudioClient.h>
#include <mmdeviceapi.h>
#include "IAudioRenderer.h"


// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000


class WasapiRenderer : public IAudioRenderer
{

private:
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	const IID IID_IAudioClient = __uuidof(IAudioClient);
	const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

	IMMDeviceEnumerator *_enumerator = NULL;
	IMMDevice *_device = NULL;
	LPWSTR _newDeviceId = new WCHAR[100];
	LPWSTR _lastDeviceId = new WCHAR[100];

	IAudioClient *_audioClient = NULL;
	IAudioRenderClient *_renderClient = NULL;
	WAVEFORMATEX *_pwfx = NULL;

	REFERENCE_TIME _requestedDuration = REFTIMES_PER_SEC / 10;
	REFERENCE_TIME _actualDuration;

	UINT32 _bufferFrameCount;
	BYTE *_data;


public:
	WasapiRenderer();
	~WasapiRenderer();

	bool Init(BaseAudioEngine *engine) override;
	bool GetFormat(WORD &numChannels, DWORD &sampleRate, WORD &bitsPerSample) override;
	bool GetBuffer(BYTE *&buffer, UINT32 &length) override;
	bool ReleaseBuffer(UINT32 numFrames, DWORD flags) override;

	bool ActivateClient();
	void ReleaseClient();
};
