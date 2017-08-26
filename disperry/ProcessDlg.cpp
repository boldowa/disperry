// processdlg.cpp : CProcessDlg ƒNƒ‰ƒX‚ÌŽÀ‘•
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Enviroment.h"
#include "Puts.h"
#include "ProcessDlg.h"

#define _S(s) _T(#s)

LRESULT CProcessDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

void CProcessDlg::Init(const CString cmdline)
{
	this->cmdline = cmdline;
}

LRESULT CProcessDlg::OnShowDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Execute dispel
	DWORD dwId;
	this->hThread = ::CreateThread(NULL, 0, &CProcessDlg::DisassembleThread, static_cast<LPVOID>(this), 0, &dwId);

	bHandled = false;
	return FALSE;
}

LRESULT CProcessDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Abort dispel
	::EnumWindows(ExitDispelCallback, (LPARAM)(&this->dispelPI));
	if(WAIT_TIMEOUT == ::WaitForSingleObject(this->dispelPI.hProcess, 100))
	{
		::TerminateProcess(this->dispelPI.hProcess, 0);
	}
	// Abort thread
	if(WAIT_TIMEOUT == ::WaitForSingleObject(this->hThread, 100))
	{
		::TerminateThread(this->hThread, 0);
	}
	// end dialog
	EndDialog(wID);
	return 0;
}

DWORD WINAPI CProcessDlg::DisassembleThread(LPVOID lpArg)
{
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	CProcessDlg *self = static_cast<CProcessDlg*>(lpArg);
	TCHAR buf[2048];
	HANDLE hRead, hWrite;
	HANDLE hERead, hEWrite;

	ZeroMemory(buf, 2048);
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&self->dispelPI, sizeof(self->dispelPI));
	si.cb = sizeof(si);

	// Create Pipe
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	::CreatePipe(&hRead, &hWrite, &sa, 0);
	::CreatePipe(&hERead, &hEWrite, &sa, 0);

	// Hide command prompt
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	// set stdout
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = hWrite;
	si.hStdError = hEWrite;

	// execute dispel
	::SetLastError(0);
	if(!CreateProcess(Enviroment::getDispelPath(), (TCHAR*)(self->cmdline.GetString()),
		NULL, NULL, TRUE,
		NORMAL_PRIORITY_CLASS,
		NULL, NULL,
		&si, &self->dispelPI))
	{
		Disperry::PutWinAPIError(self->m_hWnd, _T(__func__), _T("CreateProcess"));
		self->EndDialog(0);
		ExitThread(FALSE);
	}

	DWORD len;
	CString out = _T("");
	DWORD dwR;
	while( WAIT_ABANDONED != (dwR = WaitForSingleObject(self->dispelPI.hProcess, 50)))
	{
		::PeekNamedPipe(hRead, NULL, 0, NULL, &len, NULL);
		if(0 != len)
		{
			//Disperry::PutInfo(self->m_hWnd, PERRY_DEBUG_INT, _S(len), len);
			::ZeroMemory(buf, 2048);
#ifdef UNICODE
			char bbuf[1024];
			ZeroMemory(bbuf, 1024);
			size_t bl;
			::ReadFile(hRead, bbuf, 1023, &len, NULL);
			mbstowcs_s(&bl, buf, 2048, bbuf, strlen(bbuf));
#else
			::ReadFile(hRead, buf, 2047, &len, NULL);
#endif
			//Disperry::PutInfo(self->m_hWnd, PERRY_DEBUG_STR, _S(buf), buf);
			out += buf;
		}
		if(WAIT_OBJECT_0 == dwR) break;
	}

	DWORD result;
	::GetExitCodeProcess(self->dispelPI.hProcess, &result);
	if(0 != result) Disperry::PutError(self->m_hWnd, PERRY_FAILED_DISASSEMBLE, out);

	CloseHandle(hWrite);
	CloseHandle(hEWrite);
	CloseHandle(hRead);
	CloseHandle(hERead);
	CloseHandle(self->dispelPI.hProcess);

	self->EndDialog(0);
	ExitThread(TRUE);
}

BOOL CALLBACK CProcessDlg::ExitDispelCallback(HWND hWnd, LPARAM lParam)
{
	DWORD dwPID;
	PROCESS_INFORMATION *pi = (PROCESS_INFORMATION*)(lParam);

	// search dispel process id
	::GetWindowThreadProcessId(hWnd, &dwPID);
	if(pi->dwProcessId == dwPID)
	{
		// abort dispel
		::PostMessage(hWnd, WM_CLOSE, 0,0);
		return FALSE;
	}
	return TRUE;
}