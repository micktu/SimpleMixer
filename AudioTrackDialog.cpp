#include "AudioTrackDialog.h"
#include "CompressorTrack.h"
#include "LowpassTrack.h"


AudioTrackDialog::AudioTrackDialog()
{
}


AudioTrackDialog::~AudioTrackDialog()
{
}


void AudioTrackDialog::Init(HWND hWnd, AudioTrack * track)
{
	_audioTrack = track;
	_dialogWindow = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SOURCETRACKBAR), hWnd, DlgProc);
	SetWindowLong(_dialogWindow, GWL_USERDATA, (LONG)this);
	
	SendDlgItemMessage(_dialogWindow, IDC_VOLUME_SLIDER, TBM_SETPOS, 1, (UINT32)(_audioTrack->GetGain(0) * 100.0));
	SendDlgItemMessage(_dialogWindow, IDC_DELAY_SLIDER, TBM_SETPOS, 1, (UINT32)((1.0 -_audioTrack->GetGain(1)) * 100.0));

	LPCWSTR muteButtonText = track->IsActive() ? L"Mute" : L"Unmute";
	SendDlgItemMessage(_dialogWindow, IDC_MUTE_BUTTON, WM_SETTEXT, 0, (LPARAM)muteButtonText);

	LPCWSTR playButtonText = track->IsPlaying() ? L"Pause" : L"Play";
	SendDlgItemMessage(_dialogWindow, IDC_PLAY_BUTTON, WM_SETTEXT, 0, (LPARAM)playButtonText);

	CompressorTrack * comp = GetTrackCompressor();
	if (nullptr != comp)
	{
		SendDlgItemMessage(_dialogWindow, IDC_COMP_SLIDER, TBM_SETPOS, 1, (UINT32)(comp->GetGain(0) * 100.0));
	}

	LowpassTrack * filt = GetTrackFilter();
	if (nullptr != filt)
	{
		SendDlgItemMessage(_dialogWindow, IDC_FILTER_SLIDER, TBM_SETPOS, 1, (UINT32)((0.5 - filt->Cutoff) * 200.0));
	}

	SendDlgItemMessage(_dialogWindow, IDC_CAPTION, WM_SETTEXT, 0, (LPARAM)_audioTrack->GetTitle());

	ShowWindow(_dialogWindow, SW_SHOW);
}

void AudioTrackDialog::SetMute()
{
	bool isActive = _audioTrack->IsActive();
	_audioTrack->SetActive(!isActive);
	LPCWSTR buttonText = !isActive ? L"Mute" : L"Unmute";
	SendDlgItemMessage(_dialogWindow, IDC_MUTE_BUTTON, WM_SETTEXT, 0, (LPARAM)buttonText);
}

void AudioTrackDialog::SetPlaying()
{
	bool isPlaying = _audioTrack->IsPlaying();
	_audioTrack->SetPlaying(!isPlaying);
	LPCWSTR buttonText = isPlaying ? L"Play" : L"Pause";
	SendDlgItemMessage(_dialogWindow, IDC_PLAY_BUTTON, WM_SETTEXT, 0, (LPARAM)buttonText);
}

void AudioTrackDialog::Update()
{
	SendDlgItemMessage(_dialogWindow, IDC_AMPLITUDE, PBM_SETPOS, (UINT32)(_audioTrack->AverageLoudness * 400.0), 0);
}

CompressorTrack * AudioTrackDialog::GetTrackCompressor() const
{
	if (_audioTrack->NumChildren() < 2) return nullptr;
	return static_cast<CompressorTrack *>(_audioTrack->GetChildren()[1]);
}

LowpassTrack * AudioTrackDialog::GetTrackFilter() const
{
	if (_audioTrack->NumChildren() < 1) return nullptr;
	return static_cast<LowpassTrack *>(_audioTrack->GetChildren()[0]);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	auto trackDialog = reinterpret_cast<AudioTrackDialog *>(GetWindowLong(hwnd, GWL_USERDATA));
	
	switch (Message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_PLAY_BUTTON:
			trackDialog->SetPlaying();
			break;
		case IDC_MUTE_BUTTON:
			trackDialog->SetMute();
			break;
		}
		break;
	case WM_NOTIFY:
		CompressorTrack *comp;
		LowpassTrack *filt;
		double value = 0.0;
		switch LOWORD(wParam)
		{
		case IDC_VOLUME_SLIDER:
			value = SendDlgItemMessage(hwnd, IDC_VOLUME_SLIDER, TBM_GETPOS, 0, 0);
			trackDialog->GetTrack()->SetGain(0, value / 100.0);
			break;
		case IDC_COMP_SLIDER:
			comp = trackDialog->GetTrackCompressor();
			if (nullptr == comp) break;

			value = SendDlgItemMessage(hwnd, IDC_COMP_SLIDER, TBM_GETPOS, 0, 0);
			comp->SetGain(0, value / 100.0);
			break;
		case IDC_FILTER_SLIDER:
			filt = trackDialog->GetTrackFilter();
			if (nullptr == filt) break;

			value = SendDlgItemMessage(hwnd, IDC_FILTER_SLIDER, TBM_GETPOS, 0, 0);
			filt->Cutoff = 0.5 - value / 200.0;
			break;
		case IDC_DELAY_SLIDER:
			value = SendDlgItemMessage(hwnd, IDC_DELAY_SLIDER, TBM_GETPOS, 0, 0);
			trackDialog->GetTrack()->SetGain(1, 1.0 - value / 100.0);
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
