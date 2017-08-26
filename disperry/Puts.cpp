////////////////////////////////////////////////////////////////////////////////
// Puts.cpp : 画面表示ダイアログ メインコード
//
#include "stdafx.h"
#include <stdarg.h>
#include "resource.h"
#include "ResMan.h"
#include "Puts.h"

#define GetStrRes(tgt, id) \
	{\
		va_list vl;\
		va_start(vl, id);\
		tgt = ResMan::vsgetf(id, vl);\
		va_end(vl);\
	}\

namespace Disperry {
	class PutMan {
	public:
#ifndef NDEBUG
		CString strDebug;
#endif
		CString strInfo;
		CString strErr;
		CString strWarn;
		CString strFatal;

		PutMan(void);
		~PutMan(void);
		static PutMan& getInstance(void);
	};

	PutMan::PutMan(void)
	{
#ifndef NDEBUG
		this->strDebug  = ResMan::sgetf(PERRY_DEBUG);
#endif
		this->strInfo  = ResMan::sgetf(PERRY_INFO);
		this->strWarn  = ResMan::sgetf(PERRY_WARNING);
		this->strErr   = ResMan::sgetf(PERRY_ERROR);
		this->strFatal = ResMan::sgetf(PERRY_FATAL);
	}
	PutMan::~PutMan(void)
	{
	}
	PutMan& PutMan::getInstance(void)
	{
		static PutMan instance;
		return instance;
	}

	void PutDebug(HWND hWnd, UINT uID, ...)
	{
#ifndef NDEBUG
		CString text;
		GetStrRes(text, uID);
		::MessageBox(hWnd, text, PutMan::getInstance().strDebug, (MB_OK));
#endif
	}
	void PutInfo(HWND hWnd, UINT uID, ...)
	{
		CString text;
		GetStrRes(text, uID);
		::MessageBox(hWnd, text, PutMan::getInstance().strInfo, (MB_OK | MB_ICONINFORMATION));
	}
	void PutWarn(HWND hWnd, UINT uID, ...)
	{
		CString text;
		GetStrRes(text, uID);
		::MessageBox(hWnd, text, PutMan::getInstance().strWarn, (MB_OK | MB_ICONWARNING));
	}
	void PutError(HWND hWnd, UINT uID, ...)
	{
		CString text;
		GetStrRes(text, uID);
		::MessageBox(hWnd, text, PutMan::getInstance().strErr, (MB_OK | MB_ICONERROR));
	}
	void PutFatal(HWND hWnd, UINT uID, ...)
	{
		CString text;
		GetStrRes(text, uID);
		::MessageBox(hWnd, text, PutMan::getInstance().strFatal, (MB_OK | MB_ICONSTOP));
	}

	void PutWinAPIError(HWND hWnd, const TCHAR* caller, const TCHAR* api)
	{
		CString text;
		DWORD errCode;
		TCHAR* msg = NULL;

		errCode = ::GetLastError();
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,
			errCode, LANG_USER_DEFAULT, (LPTSTR)&msg,
			0, NULL);

		text = ResMan::sgetf(PERRY_WINAPI_FAILED, errCode, msg, caller, api);
		::MessageBox(hWnd, text, PutMan::getInstance().strFatal, (MB_OK | MB_ICONSTOP));
	}
}