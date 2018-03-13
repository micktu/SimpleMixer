#include "common.h"
#include "WasapiRenderer.h"
#include "BaseAudioEngine.h"

WasapiRenderer::WasapiRenderer()
{

}

WasapiRenderer::~WasapiRenderer()
{
	_audioClient->Stop();
	
	CoTaskMemFree(_pwfx);
	SAFE_RELEASE(_enumerator);
	ReleaseClient();
}

bool WasapiRenderer::Init(BaseAudioEngine *engine)
{
	_engine = engine;
	if (NULL == _engine) return false;

	HRESULT hr;

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&_enumerator);
	FALSE_ON_ERROR(hr)

	return ActivateClient();
}

bool WasapiRenderer::GetFormat(WORD &numChannels, DWORD &sampleRate, WORD &bitsPerSample)
{
	HRESULT hr;

	hr = _audioClient->GetMixFormat(&_pwfx);
	FALSE_ON_ERROR(hr);

	if (WAVE_FORMAT_EXTENSIBLE == _pwfx->wFormatTag)
	{
		auto x = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(_pwfx);
		if (KSDATAFORMAT_SUBTYPE_IEEE_FLOAT != x->SubFormat)
		{
			MessageBox(NULL, L"Only IEEE_FLOAT format supported.\nTell Michael about this, and try another output device.", L"Oops!", 0);
		}
	}
	else if (WAVE_FORMAT_IEEE_FLOAT != _pwfx->wFormatTag)
	{
		MessageBox(NULL, L"Only IEEE_FLOAT format supported.\nTell Michael about this, and try another output device.", L"Oops!", 0);
	}

	numChannels = _pwfx->nChannels;
	sampleRate = _pwfx->nSamplesPerSec;
	bitsPerSample = _pwfx->wBitsPerSample;
		
	return true;
}

bool WasapiRenderer::GetBuffer(BYTE *&buffer, UINT32 &length)
{
	HRESULT hr;

	UINT32 numFramesPadding;
	hr = _audioClient->GetCurrentPadding(&numFramesPadding);
	if (AUDCLNT_E_DEVICE_INVALIDATED == hr)
	{
		ReleaseClient();
		ActivateClient();
	}
	FALSE_ON_ERROR(hr)

	IMMDevice *device = NULL;
	hr = _enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
	FALSE_ON_ERROR(hr)

	device->GetId(&_newDeviceId);
	
	if (0 != wcscmp(_newDeviceId, _lastDeviceId))
	{
		ReleaseClient();
		ActivateClient();
		return false;
	}

	UINT32 numFramesAvailable;
	numFramesAvailable = _bufferFrameCount - numFramesPadding;

	hr = _renderClient->GetBuffer(numFramesAvailable, &_data);
	RETURN_ON_ERROR(hr)

	buffer = _data;
	length = numFramesAvailable;

	return true;
}

bool WasapiRenderer::ReleaseBuffer(UINT32 numFrames, DWORD flags)
{
	HRESULT hr = _renderClient->ReleaseBuffer(numFrames, flags);
	RETURN_ON_ERROR(hr)

	return true;
}

bool WasapiRenderer::ActivateClient()
{
	HRESULT hr;

	hr = _enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_device);
	FALSE_ON_ERROR(hr)

	hr = _device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&_audioClient);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->GetMixFormat(&_pwfx);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, _requestedDuration, 0, _pwfx, NULL);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->GetBufferSize(&_bufferFrameCount);
	FALSE_ON_ERROR(hr)

	_actualDuration = (double)REFTIMES_PER_SEC * _bufferFrameCount / _pwfx->nSamplesPerSec;

	hr = _audioClient->GetService(IID_IAudioRenderClient, (void**)&_renderClient);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->Start();
	FALSE_ON_ERROR(hr)

	_device->GetId(&_lastDeviceId);

	return true;
}

void WasapiRenderer::ReleaseClient()
{
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_audioClient);
	SAFE_RELEASE(_renderClient);
}



