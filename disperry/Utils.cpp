#include "stdafx.h"
#include "Utils.h"

namespace Utils {

	bool IsHex(TCHAR ch)
	{
		if(_istdigit(ch))
		{
			return true;
		}

		ch = _totupper(ch);
		if(_T('A') <= ch && _T('F') >= ch)
		{
			return true;
		}

		return false;
	}

	UINT AsciiToHex(const TCHAR* str)
	{
		size_t len;
		size_t i;
		UINT hex = 0;
		TCHAR ch;

		len = _tcslen(str);
		for(i=0; (i<len) && (IsHex(str[i])); i++)
		{
			hex <<= 4;
			if(_istdigit(str[i]))
			{
				hex += str[i] - _T('0');
				continue;
			}

			ch = _totupper(str[i]);
			hex += (ch - _T('A')) + 10;
		}

		return hex;
	}

	BOOL FileDialog(FileDialogMode mode, HWND hWnd, TCHAR* szPath, size_t pathMax, const TCHAR* filter)
	{
		OPENFILENAME ofn;
		
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);

		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = pathMax;

		switch(mode)
		{
		case FileDialog_Open:
			ofn.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
			return GetOpenFileName(&ofn);

		case FileDialog_Save:
			ofn.Flags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
			return GetSaveFileName(&ofn);
		}

		return FALSE;
	}

	CString Utils::HexAscii(int i)
	{
		static const _TCHAR AsciiTable[] = {
			_T('0'), _T('1'), _T('2'), _T('3'), 
			_T('4'), _T('5'), _T('6'), _T('7'), 
			_T('8'), _T('9'), _T('A'), _T('B'), 
			_T('C'), _T('D'), _T('E'), _T('F'), 
		};
		CString s;
		if(i == 0)
		{
			s = _T("0");
			return s;
		}

		while(i != 0)
		{
			s.Insert(0, AsciiTable[i & 0x0f]);
			i >>= 4;
		}

		return s;
	}
}
