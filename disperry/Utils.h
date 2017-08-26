#pragma once

namespace Utils {
	typedef enum {
		FileDialog_Open,
		FileDialog_Save,
	} FileDialogMode;

	bool IsHex(TCHAR);
	UINT AsciiToHex(const TCHAR*);
	BOOL FileDialog(FileDialogMode mode, HWND hWnd, TCHAR* szPath, size_t pathMax, const TCHAR* filter);
	CString HexAscii(int);
}
