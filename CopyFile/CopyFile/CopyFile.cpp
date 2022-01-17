#include <Windows.h>
#include <iostream>

#define COPY_BUFFER_SIZE (6 * 1024)

#define SRC_FILE L"C:\\ProgramData\\InterAmplifier\\Config\\template.yaml"
#define DST_FILE L"C:\\Users\\Tom\\AppData\\Roaming\\InterAmplifier\\Render\\Multimedia\\{TEST}"


#define _PRINT(severuty, ...)  fprintf(stderr, __VA_ARGS__)

HRESULT CopyFile(PWSTR DstPath, PWSTR SrcPath);

HRESULT CopyFile(PWSTR DstPath, PWSTR SrcPath)
{
    HANDLE hSrc, hDst;
	HRESULT hr = S_OK;
    WCHAR buffer[COPY_BUFFER_SIZE];
	BOOL status = FALSE;
	DWORD nNumberOfBytesToRead = COPY_BUFFER_SIZE;
	//DWORD lpNumberOfBytesRead;
	DWORD nNumberOfBytesToWrite = 0;
	//DWORD lpNumberOfBytesWritten;

	hSrc = CreateFileW(SrcPath,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hSrc == INVALID_HANDLE_VALUE) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		_PRINT(_DBG_ERROR, "SRC CreateFile failed<%ws> 0x%08X %s\n", SrcPath, hr,
			hr == 0x80070020 ? "(This file is locked!)" : "");
		return hr;
	}

	hDst = CreateFileW(DstPath,
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hDst == INVALID_HANDLE_VALUE) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		_PRINT(_DBG_ERROR, "DST CreateFile failed<%ws> 0x%08X %s\n", DstPath, hr,
			hr == 0x80070020 ? "(This file is locked!)" : "");
		return hr;
	}

	status = ReadFile(
		hSrc, //HANDLE,
		buffer, //LPVOID,
		nNumberOfBytesToRead,
		&nNumberOfBytesToWrite,
		NULL
	);
	if (!status) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		_PRINT(_DBG_ERROR, "ReadFile failed=0x%08X\n", hr);
		return hr;
	}

	_PRINT(_DBG_ERROR, "Read Length=%lu\n", nNumberOfBytesToWrite);

	status = WriteFile(
		hDst, //HANDLE,
		buffer, //LPVOID,
		nNumberOfBytesToWrite,
		NULL,
		NULL
	);
	if (!status) {
		hr = HRESULT_FROM_WIN32(GetLastError());
		_PRINT(_DBG_ERROR, "WriteFile failed=0x%08X\n", hr);
		return hr;
	}

	CloseHandle(hSrc);
	CloseHandle(hDst);

    return hr;
}

int main()
{
    std::cout << "Hello CopyFile!\n";

    CopyFile((PWSTR) DST_FILE, (PWSTR) SRC_FILE);

}

