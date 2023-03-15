#pragma once
#ifdef _rjOS_WIN
#include <cstdint>
#include <windows.h>
 bool APIENTRY DllMain(HANDLE hModule, uint32_t reasonForCall, LPVOID) {
	switch (reasonForCall) {
	case DLL_PROCESS_ATTACH:DisableThreadLibraryCalls((HINSTANCE) hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:break;
	}
	return true;
}
#endif 

