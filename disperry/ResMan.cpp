#include "StdAfx.h"
#include <stdarg.h>
#include "ResMan.h"

HINSTANCE ResMan::m_hInstance = NULL;

ResMan::ResMan(void){}

void ResMan::SetInstanceHandle(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

CString ResMan::sgetf(UINT uID, ...)
{
	TCHAR resbuf[1024];
	TCHAR strbuf[1024];
	va_list vl;
	if(!::LoadString(m_hInstance, uID, resbuf, 1024))
	{
		return CString(_T("(Nothing)"));
	}

	va_start(vl, uID);
	_vstprintf_s(strbuf, 1024, resbuf, vl);
	va_end(vl);

	return CString(strbuf);
}

CString ResMan::vsgetf(UINT uID, va_list vl)
{
	TCHAR resbuf[1024];
	TCHAR strbuf[8192];
	if(!::LoadString(m_hInstance, uID, resbuf, 1024))
	{
		return CString(_T("(Nothing)"));
	}

	_vstprintf_s(strbuf, 8192, resbuf, vl);

	return CString(strbuf);
}