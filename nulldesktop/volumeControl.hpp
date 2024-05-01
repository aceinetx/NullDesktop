#include "include.h"
IMMDeviceEnumerator* deviceEnumerator = NULL;
IMMDevice* defaultDevice = NULL;
HRESULT hr;
IAudioEndpointVolume* endpointVolume = NULL;

namespace volumeControl {

	void InitDevice() {
		hr = CoInitialize(NULL);
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL;
	}

	float GetCurrentVolume() {
		if (endpointVolume == NULL) throw std::runtime_error("Volume control: Device not initalized (endpointVolume == NULL)");
		float vol;
		endpointVolume->GetMasterVolumeLevelScalar(&vol);
		vol *= 100;
		return vol;
	}

	void SetCurrentVolume(float vol) {
		endpointVolume->SetMasterVolumeLevelScalar(vol / 100, NULL);
	}

	bool IsMuted() {
		BOOL muted;
		endpointVolume->GetMute(&muted);
		return muted;
	}

	void Mute(BOOL state) {
		endpointVolume->SetMute(state, NULL);
	}
}