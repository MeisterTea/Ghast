#include <windows.h>
#include <mmsystem.h>
#include <ctime>
#include <Endpointvolume.h>
#include <Winuser.h>
#include "resource.h"
#include <Mfidl.h>
#include <Audioclient.h>
#include <mmdeviceapi.h>

HRESULT hr;
float voriginal;
BOOL muteoriginal;

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       cmdShow)
{
	// Initializes IAudioEndpointVolume
	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	while (true)
	{
		// 5 to 10 minutes wait
		srand((unsigned int)time(NULL));
		Sleep(rand() % 900000 + 300000);

		// Gets Beforehand status
		hr = endpointVolume->GetMute(&muteoriginal); // Gets Mute state
		hr = endpointVolume->GetMasterVolumeLevelScalar(&voriginal); // Gets Master Volume

		
		hr = endpointVolume->SetMasterVolumeLevelScalar(0.5, NULL); // Sets Master Volume to 50%
		hr = endpointVolume->SetMute(0, NULL); // Unmutes

		Sleep(50);

		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE);

		// Sets back Master Volume
		hr = endpointVolume->SetMasterVolumeLevelScalar(voriginal, NULL);

		// Sets back Mute state
		hr = endpointVolume->SetMute(muteoriginal, NULL);
	}
}
