#include "common.h"
#include "WasapiRenderer.h"
#include "IAudioEngine.h"

WasapiRenderer::WasapiRenderer()
{
}

WasapiRenderer::~WasapiRenderer()
{
	_audioClient->Stop();
	
	CoTaskMemFree(_pwfx);
	SAFE_RELEASE(_enumerator)
	SAFE_RELEASE(_device)
	SAFE_RELEASE(_audioClient)
	SAFE_RELEASE(_renderClient)
}

bool WasapiRenderer::Init(IAudioEngine *engine)
{
	_engine = engine;
	if (NULL == _engine) return false;

	HRESULT hr;

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&_enumerator);
	FALSE_ON_ERROR(hr)

	hr = _enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_device);
	FALSE_ON_ERROR(hr)

	hr = _device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&_audioClient);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->GetMixFormat(&_pwfx);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,	0, _requestedDuration, 0,	_pwfx, NULL);
	FALSE_ON_ERROR(hr)

	//hr = pMySource->SetFormat(pwfx);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->GetBufferSize(&_bufferFrameCount);
	FALSE_ON_ERROR(hr)

	hr = _audioClient->GetService(IID_IAudioRenderClient, (void**)&_renderClient);
	FALSE_ON_ERROR(hr)

	DWORD flags = 0;
	//hr = _fillBuffer(_bufferFrameCount, flags);
	FALSE_ON_ERROR(hr)

	_actualDuration = (double)REFTIMES_PER_SEC * _bufferFrameCount / _pwfx->nSamplesPerSec;

	hr = _audioClient->Start();
	FALSE_ON_ERROR(hr)

	return true;
}

bool WasapiRenderer::Render()
{
	HRESULT hr;

	UINT32 numFramesPadding;
	hr = _audioClient->GetCurrentPadding(&numFramesPadding);
	FALSE_ON_ERROR(hr)

	UINT32 numFramesAvailable;
	numFramesAvailable = _bufferFrameCount - numFramesPadding;

	DWORD flags = 0;
	hr = _fillBuffer(numFramesAvailable, flags);
	FALSE_ON_ERROR(hr)

	return AUDCLNT_BUFFERFLAGS_SILENT != flags;
}

bool WasapiRenderer::GetFormat(WORD &numChannels, DWORD &sampleRate, WORD &bitsPerSample)
{
	HRESULT hr = _audioClient->GetMixFormat(&_pwfx);

	if (WAVE_FORMAT_EXTENSIBLE == _pwfx->wFormatTag)
	{
		auto x = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(_pwfx);
		DEBUG_VALUE(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT == x->SubFormat);
	}

	FALSE_ON_ERROR(hr);

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
	FALSE_ON_ERROR(hr)

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

HRESULT WasapiRenderer::_fillBuffer(UINT32 numFrames, DWORD &flags)
{
	HRESULT hr;

	hr = _renderClient->GetBuffer(numFrames, &_data);
	RETURN_ON_ERROR(hr)

	//hr = pMySource->LoadData(numFramesAvailable, pData, &flags);
	RETURN_ON_ERROR(hr)

	hr = _renderClient->ReleaseBuffer(numFrames, flags);
	RETURN_ON_ERROR(hr)
}


