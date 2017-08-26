// processdlg.h : CProcessDlg クラスのインターフェイス
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CProcessDlg : public CDialogImpl<CProcessDlg>
{
private:
	PROCESS_INFORMATION dispelPI;
	HANDLE hThread;
	CString cmdline;

	void ShowError(CString err);
	static DWORD WINAPI DisassembleThread(LPVOID lpArg);
	static BOOL CALLBACK ExitDispelCallback(HWND hWnd, LPARAM lParam);
public:
	enum { IDD = IDD_PROCESSBOX };

	bool isFail;

	void Init(const CString);

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowDialog)
		COMMAND_ID_HANDLER(IDABORT, OnCloseCmd)
	END_MSG_MAP()

	// ハンドラーのプロトタイプ （引数が必要な場合はコメントを外してください）:
	//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
