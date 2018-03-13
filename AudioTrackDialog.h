#pragma once

#include "common.h"
#include "resource.h"
#include "AudioTrack.h"

class AudioTrackDialog
{
private:
	HWND _dialogWindow = nullptr;
	AudioTrack *_audioTrack = nullptr;

public:
	AudioTrackDialog();
	~AudioTrackDialog();

	void Init(HWND hWnd, AudioTrack *track);

	void SetMute();
	void SetPlaying();

	void Update();

	class CompressorTrack * GetTrackCompressor() const;
	class LowpassTrack * GetTrackFilter() const;

	__forceinline HWND GetWindow() const { return _dialogWindow; }
	__forceinline AudioTrack * GetTrack() const { return _audioTrack; }
};

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
