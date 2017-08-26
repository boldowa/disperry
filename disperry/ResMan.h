#pragma once

class ResMan
{
private:
	static HINSTANCE m_hInstance;
	// static class
	ResMan(void);
public:
	static void SetInstanceHandle(HINSTANCE);
	static CString sgetf(UINT uID, ...);
	static CString vsgetf(UINT uID, va_list vl);
};
