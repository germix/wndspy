#include <windows.h>

typedef BOOL  (WINAPI * EnumProcessModulesProc )(HANDLE, HMODULE *, DWORD, LPDWORD);
typedef DWORD (WINAPI * GetModuleBaseNameProc  )(HANDLE, HMODULE, LPTSTR, DWORD);
typedef DWORD (WINAPI * GetModuleFileNameExProc)(HANDLE, HMODULE, LPTSTR, DWORD);

//
// This uses PSAPI.DLL, which is only available under NT/2000/XP I think,
// so we dynamically load this library, so that we can still run under 9x.
//
//	dwProcessId  [in]
//  szName       [out]
//  nNameSize    [in]
//  szPath       [out]
//  nPathSize    [in]
//
BOOL GetProcessNameByPid(DWORD dwProcessId, WCHAR szName[], DWORD nNameSize, WCHAR szPath[], DWORD nPathSize)
{
	HMODULE hPSAPI;
	HANDLE hProcess;

	HMODULE hModule;
	DWORD   dwNumModules;

	EnumProcessModulesProc  fnEnumProcessModules;
	GetModuleBaseNameProc   fnGetModuleBaseName;
	GetModuleFileNameExProc fnGetModuleFileNameEx;

	// Attempt to load Process Helper library
	hPSAPI = LoadLibrary(L"psapi.dll");

	if(!hPSAPI)
	{
		szName[0] = '\0';
		return FALSE;
	}

	// OK, we have access to the PSAPI functions, so open the process
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);

	if(!hProcess)
	{
		FreeLibrary(hPSAPI);
		return FALSE;
	}


	fnEnumProcessModules  = (EnumProcessModulesProc)GetProcAddress(hPSAPI, "EnumProcessModules");
#if 1	//!!!
	fnGetModuleBaseName   = (GetModuleBaseNameProc)  GetProcAddress(hPSAPI, "GetModuleBaseNameW");
	fnGetModuleFileNameEx = (GetModuleFileNameExProc)GetProcAddress(hPSAPI, "GetModuleFileNameExW");
#else
#ifdef UNICODE
	fnGetModuleBaseName   = (GetModuleBaseNameProc)  GetProcAddress(hPSAPI, "GetModuleBaseNameW");
	fnGetModuleFileNameEx = (GetModuleFileNameExProc)GetProcAddress(hPSAPI, "GetModuleFileNameExW");
#else
	fnGetModuleBaseName   = (GetModuleBaseNameProc)  GetProcAddress(hPSAPI, "GetModuleBaseNameA");
	fnGetModuleFileNameEx = (GetModuleFileNameExProc)GetProcAddress(hPSAPI, "GetModuleFileNameExA");
#endif
#endif
	if(!fnEnumProcessModules || !fnGetModuleBaseName)
	{
		CloseHandle(hProcess);
		FreeLibrary(hPSAPI);
		return FALSE;
	}

	// Find the first module
	if(fnEnumProcessModules(hProcess, &hModule, sizeof(hModule), &dwNumModules))
	{
		// Now get the module name
		if(szName)
			fnGetModuleBaseName(hProcess, hModule, szName, nNameSize);

		// get module filename
		if(szPath)
			fnGetModuleFileNameEx(hProcess, hModule, szPath, nPathSize);
	}
	else
	{
		szName[0] = '\0';
		szPath[0] = '\0';
	}

	CloseHandle(hProcess);
	FreeLibrary(hPSAPI);

	return TRUE;
}



