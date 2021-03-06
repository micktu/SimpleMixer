// SimpleMixer.cpp : Defines the entry point for the application.
//

#include "common.h"
#include "SimpleMixer.h"
#include "MaxiAudioEngine.h"
#include "WasapiRenderer.h"
#include "AudioTrackDialog.h"
#include "SineGenerator.h"
#include "SamplePlayer.h"
#include "CompressorTrack.h"
#include "DelayTrack.h"
#include "LowpassTrack.h"

#define MAX_LOADSTRING 100
#define SECONDS_PER_FRAME 0.02

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// My global variables
BaseAudioEngine *_audioEngine = NULL;
std::vector<AudioTrackDialog *> _trackDialogs;

void SetupAudio(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SIMPLEMIXER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 460 + 12, 120 * 4 + 40, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd) return -1;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEMIXER));

	Utils::SetPathToExecutableA();
	
	// Initial timer values
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	float frequency = (float)li.QuadPart;
	QueryPerformanceCounter(&li);
	float lastTime = (float)li.QuadPart / frequency;

	SetupAudio(hWnd);
	
	MSG msg;

	// Naive fixed timestep loop
	bool isRunning = true;
	ULONG tickCounter = 0;
	while (isRunning)
	{
		QueryPerformanceCounter(&li);
		float startTime = (float)li.QuadPart / frequency;
		float deltaTime = SECONDS_PER_FRAME;

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (WM_QUIT == msg.message) isRunning = false;
			continue;
		}

		if (nullptr == _audioEngine) continue;

		tickCounter++;
		_audioEngine->Update();

		for (AudioTrackDialog *dialog : _trackDialogs)
		{
			dialog->Update();
		}

		QueryPerformanceCounter(&li);
		float endTime = (float)li.QuadPart / frequency;
		float timeToSleep = SECONDS_PER_FRAME - (endTime - startTime);
		if (timeToSleep > 0.0) Sleep((DWORD)(timeToSleep * 1000.0));
	}

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style =          CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLEMIXER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SIMPLEMIXER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		break;
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
		PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void SetupAudio(HWND hWnd)
{
	_audioEngine = BaseAudioEngine::InitAudioEngine<MaxiAudioEngine, WasapiRenderer>();

	auto sampleTrack = _audioEngine->AddTrack<SamplePlayer>();
	sampleTrack->SetFile("Assets\\game3-mobile-map-preview.ogg");
	sampleTrack->SetGain(0, 1.0);
	sampleTrack->SetGain(1, 1.0);
	sampleTrack->SetGain(2, 0.5);
	sampleTrack->SetTitle(L"Music");
	auto filt1 = sampleTrack->AddChild<LowpassTrack>();
	filt1->SetGain(0, 1.0);
	auto comp1 = sampleTrack->AddChild<CompressorTrack>();

	auto sineTrack = _audioEngine->AddTrack<SineGenerator>();
	sineTrack->Frequency = 349.0; // Note F4
	sineTrack->SetGain(0, 0.2);
	sineTrack->SetGain(1, 0.5);
	sineTrack->SetGain(2, 0.5);
	sineTrack->SetPlaying(false);
	sineTrack->SetTitle(L"Sine Wave");
	auto filt2 = sineTrack->AddChild<LowpassTrack>();
	filt2->SetGain(0, 1.0);
	auto comp2 = sineTrack->AddChild<CompressorTrack>();
	comp2->SetGain(0, 0.0);

	auto delayTrack = _audioEngine->AddTrack<DelayTrack>();
	delayTrack->SetGain(0, 1.0);
	delayTrack->SetTitle(L"Delay Bus");

	auto master = _audioEngine->AddTrack<AudioTrack>();
	master->SetGain(0, 1.0);
	master->SetTitle(L"Master");

	sampleTrack->AddOutput(master);
	sampleTrack->AddOutput(delayTrack);

	sineTrack->AddOutput(master);
	sineTrack->AddOutput(delayTrack);

	delayTrack->AddOutput(master);

	auto tracks = _audioEngine->GetTracks();
	for (UINT32 i = 0; i < tracks.size(); i++)
	{
		AudioTrackDialog *trackDialog = new AudioTrackDialog();
		trackDialog->Init(hWnd, tracks[i]);
		//SetWindowPos(trackDialog->GetWindow(), 0, (i % 2) * 460, (i / 2) * 120, 0, 0, SWP_NOSIZE);
		SetWindowPos(trackDialog->GetWindow(), 0, 0, i * 120, 0, 0, SWP_NOSIZE);
		_trackDialogs.push_back(trackDialog);
	}
}