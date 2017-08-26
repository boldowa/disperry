// MainDlg.cpp : CMainDlg クラスの実装
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "Puts.h"
#include "Utils.h"
#include "ResMan.h"
#include "AppConfig.h"
#include "ProcessDlg.h"
#include "aboutdlg.h"
#include "MainDlg.h"

static int _chk(bool b)
{
	return b ? BST_CHECKED : BST_UNCHECKED;
}
void CMainDlg::Init(CAppConfig* appConfig)
{
	// 設定の取得
	this->appConfig = appConfig;
	this->cfg = appConfig->Get();

	//--- アプリケーション設定の反映
	// path edit
	this->ctlInputFileEdit.SetWindowText(cfg.inputPath);
	this->ctlOutputFileEdit.SetWindowText(cfg.outputPath);
	// spin edit
	this->ctlHexUpDown.SetPos32(cfg.hexWidth);
	// address edit
	this->ctlDisassembleBankEdit.SetWindowText(cfg.disassembleBank);
	this->ctlOriginAddressEdit.SetWindowText(cfg.originAddress);
	this->ctlBeginAddressEdit.SetWindowText(cfg.beginAddress);
	this->ctlEndAddressEdit.SetWindowText(cfg.endAddress);
	// checkbox
	this->ctlEnableSkipHeaderCheck.SetCheck(_chk(cfg.enableSkipHeader));
	this->ctlDisableHexDumpCheck.SetCheck(_chk(cfg.disableHexDump));
	this->ctlIs8bitsAccumlatorCheck.SetCheck(_chk(cfg.is8bitsAccumlator));
	this->ctlIs8bitsIndexRegisterCheck.SetCheck(_chk(cfg.is8bitsIndexRegister));
	this->ctlDisableBoundaryCheck.SetCheck(_chk(cfg.disableBoundary));
	this->ctlEnableSplitCheck.SetCheck(_chk(cfg.enableSplit));
	this->ctlEnable1BankDisassembleCheck.SetCheck(_chk(cfg.enable1BankDisassemble));
	this->ctlDisableDisassembleCheck.SetCheck(_chk(cfg.disableDisassemble));
	this->ctlEnableOriginCheck.SetCheck(_chk(cfg.enableOrigin));
	this->ctlEnableRangeCheck.SetCheck(_chk(cfg.enableRange));
	// BankMap radiobox
	switch(cfg.bankMode)
	{
	case BankMode_Auto:
		this->ctlBankAutoRadio.SetCheck(BST_CHECKED);
		break;
	case BankMode_LoRom:
		this->ctlBankLoRomRadio.SetCheck(BST_CHECKED);
		break;
	case BankMode_HiRom:
		this->ctlBankHiRomRadio.SetCheck(BST_CHECKED);
		break;
	}
	// MirrorAddress radiobox
	switch(cfg.shadowMode)
	{
	case ShadowAddress_Auto:
		this->ctlRomAddressAutoRadio.SetCheck(BST_CHECKED);
		break;
	case ShadowAddress_Enable:
		this->ctlRomAddressEnableRadio.SetCheck(BST_CHECKED);
		break;
	case ShadowAddress_Disable:
		this->ctlRomAddressDisableRadio.SetCheck(BST_CHECKED);
		break;
	}
	// set limit 
	this->ctlHexWidthEdit.SetLimit(2);
	this->ctlDisassembleBankEdit.SetLimit(2);
	this->ctlOriginAddressEdit.SetLimit(6);
	this->ctlBeginAddressEdit.SetLimit(6);
	this->ctlEndAddressEdit.SetLimit(6);

	// update control enabled status
	this->UpdateEnableStatus();
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYDOWN == pMsg->message)
	{
		if( VK_ESCAPE == pMsg->wParam )
		{
			return FALSE;
		}
	}
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// 画面の中心へダイアログを移動
	CenterWindow();

	// リソースにコントロールを関連付ける
	this->AssociateControls();

	// アイコンを設定する
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// メッセージ フィルターおよび画面更新用のオブジェクト登録
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// メッセージ フィルターおよび画面更新用のオブジェクト登録解除
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnDisassemble(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// パラメータのチェック
	UINT err;
	if(0 != (err = this->Validata()))
	{
		Disperry::PutError(this->m_hWnd, PERRY_INVALID_ON_DISASM, ResMan::sgetf(err));
		return 0;
	}
	this->ApplyConfig();

	// Dispelのコマンドライン引数を作成
	CString cmdline;
	TCHAR tmp[128];
									cmdline += _T(" ");
	if(cfg.enableSkipHeader)		cmdline += _T("-n ");
	if(cfg.disableHexDump)			cmdline += _T("-t ");
	if(cfg.is8bitsAccumlator)		cmdline += _T("-a ");
	if(cfg.is8bitsIndexRegister)	cmdline += _T("-x ");
	if(cfg.disableBoundary)			cmdline += _T("-e ");
	if(cfg.enableSplit)				cmdline += _T("-p ");

	if(cfg.enable1BankDisassemble){
		_stprintf_s(tmp, 128, _T("%s "), cfg.disassembleBank.GetString());
		cmdline += _T("-b ");
		cmdline += tmp;
	}
	if(cfg.disableDisassemble)
	{
		_stprintf_s(tmp, 128, _T("%x "), cfg.hexWidth);
		cmdline += _T("-d ");
		cmdline += tmp;
	}
	if(cfg.enableOrigin)
	{
		_stprintf_s(tmp, 128, _T("%s "), cfg.originAddress.GetString());
		cmdline += _T("-g ");
		cmdline += tmp;
	}
	if(cfg.enableRange)
	{
		_stprintf_s(tmp, 128, _T("%s-%s "), cfg.beginAddress.GetString(), cfg.endAddress.GetString());
		cmdline += _T("-r ");
		cmdline += tmp;
	}

	switch(this->cfg.bankMode)
	{
	case BankMode_LoRom:
		cmdline += _T("-l ");
		break;
	case BankMode_HiRom:
		cmdline += _T("-h ");
		break;
	default:
		break;
	}
	switch(this->cfg.shadowMode)
	{
	case ShadowAddress_Enable:
		cmdline += _T("-s ");
		break;
	case ShadowAddress_Disable:
		cmdline += _T("-i ");
		break;
	default:
		break;
	}

	// output
	cmdline += _T("-o \"");
	cmdline += cfg.outputPath.GetString();

	// input
	cmdline += _T("\" \"");
	cmdline += cfg.inputPath.GetString();
	cmdline += _T("\"");

	//Disperry::PutInfo(this->m_hWnd, PERRY_DEBUG_STR, _T("cmd: "), cmdline);

	// 処理ダイアログの生成 / 逆アセンブルの実行
	CProcessDlg dlg;
	dlg.Init(cmdline);
	dlg.DoModal();

	//Disperry::PutInfo(this->m_hWnd, ID_FILE_NEW);
	return 0;
}

LRESULT CMainDlg::OnExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 内部関数
	struct nullchk{
		UINT operator()(UINT n, Controls::CCtlPathEdit& ctl)
		{
			if(n != PERRY_SAMENAME_INOUT)
			{
				return n;
			}

			// ファイル名が空の場合は、チェックなし
			CString str;
			ctl.GetWindowText(str);
			if(str.IsEmpty())
			{
				return 0;
			}
			return n;
		}
	};
	UINT err;

	if(0 != (err = Validata()))
	{
		if(0 != nullchk()(err,this->ctlOutputFileEdit))
		{
			// 終了時エラー確認
			if(IDNO == MessageBox(ResMan::sgetf(PERRY_INVALID_ON_EXIT, ResMan::sgetf(err)), ResMan::sgetf(PERRY_WARNING), MB_YESNO|MB_ICONWARNING))
			{
				return 0;
			}
		}
	}

	// 設定値の保存
	this->ApplyConfig();
	this->appConfig->Apply(cfg);

	CloseDialog(wID);
	return 0;
}

static void GetPathFromCtl(TCHAR* szPath, const int Max, CWindow& ctl)
{
	size_t len;

	ctl.GetWindowText(szPath, Max);
	len = _tcslen(szPath);
	if(_T('\\') == szPath[len-1])
	{
		szPath[len-1] = _T('\0');
	}
}

LRESULT CMainDlg::OnSelectInputFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static const TCHAR* filter = _T("SNES ROM file(*.smc,*.sfc,*.bin)\0*.smc;*.sfc;*.bin\0All file(*.*)\0*.*\0\0");
	/*
	CFileDialog dlg(TRUE,0,0,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,filter,NULL);
	return SetFilePathEdit(dlg, this->ctlInputFileEdit);
	*/
	TCHAR szPath[MAX_PATH] = {0};
	GetPathFromCtl(szPath, MAX_PATH, this->ctlInputFileEdit);
	if(Utils::FileDialog(Utils::FileDialog_Open, this->m_hWnd, szPath, MAX_PATH, filter))
	{
		this->ctlInputFileEdit.SetWindowText(szPath);
	}
	if(::GetLastError())
	{
		Disperry::PutWinAPIError(this->m_hWnd, _T(__func__), _T("GetOpenFileName"));
	}
	return 0;
}
LRESULT CMainDlg::OnSelectOutputFile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static const TCHAR* filter = _T("ASM file(*.asm)\0*.asm\0All file(*.*)\0*.*\0\0");
	/*
	CFileDialog dlg(TRUE,0,0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter,NULL);
	return SetFilePathEdit(dlg, this->ctlOutputFileEdit);
	*/
	TCHAR szPath[MAX_PATH] = {0};
	GetPathFromCtl(szPath, MAX_PATH, this->ctlOutputFileEdit);
	if(Utils::FileDialog(Utils::FileDialog_Save, this->m_hWnd, szPath, MAX_PATH, filter))
	{
		this->ctlOutputFileEdit.SetWindowText(szPath);
	}
	if(::GetLastError())
	{
		Disperry::PutWinAPIError(this->m_hWnd, _T(__func__), _T("GetSaveFileName"));
	}
	return 0;
}
/*
LRESULT CMainDlg::SetFilePathEdit(CFileDialog& dlg, Controls::CCtlPathEdit& ctl)
{
	int result;

	result = dlg.DoModal();
	if(result == IDOK)
	{
		ctl.SetWindowText(dlg.m_szFileName);
	}
	else if(result != IDCANCEL)
	{
		Disperry::PutError(this->m_hWnd, PERRY_UNKNOWN_ERROR);
	}
	return 0;
}
*/

LRESULT CMainDlg::OnEditInputPath(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(EN_CHANGE != wNotifyCode)
	{
		return 0;
	}
	return this->GenerateOutputPathFromInputPath();
}

LRESULT CMainDlg::OnEditHexWidth(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(EN_CHANGE != wNotifyCode)
	{
		return 0;
	}
	return this->UpdateHexWidthConfig();
}

LRESULT CMainDlg::OnCheckBoxClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// BankMode
	if((IDC_RADIO_BM_AUTO <= wID) && (IDC_RADIO_BM_HIROM >= wID))
	{
		return ChangeBankMapMode(wID);
	}

	// ShadowAddressMode
	if((IDC_RADIO_SA_AUTO <= wID) && (IDC_RADIO_SA_DISABLE >= wID))
	{
		return ChangeShadowAddressMode(wID);
	}

	// check box
	return ChangeCheckBoxStatus(wID);
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::AssociateControls(void)
{
	// パス入力コントロール
	this->ctlInputFileEdit.SubclassWindow(this->GetDlgItem(IDC_EDIT_INPUT));
	this->ctlOutputFileEdit.SubclassWindow(this->GetDlgItem(IDC_EDIT_OUTPUT));

	// HEXダンプ幅入力コントロール
	this->ctlHexUpDown = this->GetDlgItem(IDC_SPIN_HEXWIDTH);
	this->ctlHexUpDown.SetRange32(1, 64);

	// アドレス入力コントロール
	this->ctlBeginAddressEdit.SubclassWindow(this->GetDlgItem(IDC_EDIT_START));
	this->ctlEndAddressEdit.SubclassWindow(this->GetDlgItem(IDC_EDIT_END));
	this->ctlDisassembleBankEdit.SubclassWindow(this->GetDlgItem(IDC_EDIT_BANK));
	this->ctlOriginAddressEdit.SubclassWindow(this->GetDlgItem(IDC_EDIT_ORIGIN_ADR));
	this->ctlHexWidthEdit.SubclassWindow(this->GetDlgItem(IDC_EDIT_HEXWIDTH));

	// チェックボックス コントロール
	this->ctlEnableSkipHeaderCheck = this->GetDlgItem(IDC_CHECK_HEADER);
	this->ctlDisableHexDumpCheck = this->GetDlgItem(IDC_CHECK_NOHEX);
	this->ctlIs8bitsAccumlatorCheck = this->GetDlgItem(IDC_CHECK_ACCUM);
	this->ctlIs8bitsIndexRegisterCheck = this->GetDlgItem(IDC_CHECK_INDEX);
	this->ctlDisableBoundaryCheck = this->GetDlgItem(IDC_CHECK_BOUNDARY);
	this->ctlEnableSplitCheck = this->GetDlgItem(IDC_CHECK_SPLIT);
	this->ctlEnable1BankDisassembleCheck = this->GetDlgItem(IDC_CHECK_BANK);
	this->ctlDisableDisassembleCheck = this->GetDlgItem(IDC_CHECK_HEXONLY);
	this->ctlEnableOriginCheck = this->GetDlgItem(IDC_CHECK_ORIGIN);
	this->ctlEnableRangeCheck = this->GetDlgItem(IDC_CHECK_STARTEND);

	// ラジオボタン コントロール
	this->ctlBankAutoRadio = this->GetDlgItem(IDC_RADIO_BM_AUTO);
	this->ctlBankLoRomRadio = this->GetDlgItem(IDC_RADIO_BM_LOROM);
	this->ctlBankHiRomRadio = this->GetDlgItem(IDC_RADIO_BM_HIROM);
	this->ctlRomAddressAutoRadio = this->GetDlgItem(IDC_RADIO_SA_AUTO);
	this->ctlRomAddressEnableRadio = this->GetDlgItem(IDC_RADIO_SA_ENABLE);
	this->ctlRomAddressDisableRadio = this->GetDlgItem(IDC_RADIO_SA_DISABLE);
}

LRESULT CMainDlg::GenerateOutputPathFromInputPath(void)
{
	CPath outPath;

	this->ctlInputFileEdit.GetWindowText(outPath);
	outPath.RemoveExtension();
	outPath.AddExtension(_T(".asm"));
	this->ctlOutputFileEdit.SetWindowText(outPath);
	return 0;
}

LRESULT CMainDlg::UpdateHexWidthConfig(void)
{
	if(!this->ctlHexUpDown.IsWindow()) return 0;
	this->cfg.hexWidth = this->ctlHexUpDown.GetPos32();
	this->ctlHexUpDown.SetPos32(this->cfg.hexWidth);
	return 0;
}
LRESULT CMainDlg::ChangeBankMapMode(WORD wID)
{
	this->cfg.bankMode = static_cast<BankMode>(wID - IDC_RADIO_BM_AUTO);
	return 0;
}

LRESULT CMainDlg::ChangeShadowAddressMode(WORD wID)
{
	this->cfg.shadowMode = static_cast<ShadowAddressMode>(wID - IDC_RADIO_SA_AUTO);
	return 0;
}

LRESULT CMainDlg::ChangeCheckBoxStatus(WORD wID)
{
	// this table is made from resource.rc
	CButton *chks[] = {
		&this->ctlEnableSkipHeaderCheck,
		&this->ctlDisableHexDumpCheck,
		&this->ctlIs8bitsAccumlatorCheck,
		&this->ctlIs8bitsIndexRegisterCheck,
		&this->ctlDisableBoundaryCheck,
		&this->ctlEnableSplitCheck,
		&this->ctlEnable1BankDisassembleCheck,
		NULL,
		NULL,
		&this->ctlDisableDisassembleCheck,
		NULL,
		NULL,
		NULL,
		&this->ctlEnableOriginCheck,
		NULL,
		NULL,
		&this->ctlEnableRangeCheck,
	};
	bool *confs[] = {
		&this->cfg.enableSkipHeader,
		&this->cfg.disableHexDump,
		&this->cfg.is8bitsAccumlator,
		&this->cfg.is8bitsIndexRegister,
		&this->cfg.disableBoundary,
		&this->cfg.enableSplit,
		&this->cfg.enable1BankDisassemble,
		NULL,
		NULL,
		&this->cfg.disableDisassemble,
		NULL,
		NULL,
		NULL,
		&this->cfg.enableOrigin,
		NULL,
		NULL,
		&this->cfg.enableRange,
	};

	int i = (wID - IDC_CHECK_HEADER);
	ATLASSERT((chks[i] != NULL) && (confs[i] != NULL));
	(*confs[i]) = (BST_UNCHECKED != chks[i]->GetCheck());

	this->UpdateEnableStatus();

	return 0;
}

void CMainDlg::UpdateEnableStatus(void)
{
	// Check 1bank mode
	if(BST_CHECKED == this->ctlEnable1BankDisassembleCheck.GetCheck())
	{
		this->GetDlgItem(IDC_STATIC_BANK).EnableWindow(TRUE);
		this->ctlDisassembleBankEdit.EnableWindow(TRUE);
	}
	else
	{
		this->GetDlgItem(IDC_STATIC_BANK).EnableWindow(FALSE);
		this->ctlDisassembleBankEdit.EnableWindow(FALSE);
	}

	// Check hexdump mode
	if(BST_CHECKED == this->ctlDisableDisassembleCheck.GetCheck())
	{
		this->GetDlgItem(IDC_STATIC_HEX_WIDTH).EnableWindow(TRUE);
		this->GetDlgItem(IDC_EDIT_HEXWIDTH).EnableWindow(TRUE);
		this->ctlHexUpDown.EnableWindow(TRUE);
	}
	else
	{
		this->GetDlgItem(IDC_STATIC_HEX_WIDTH).EnableWindow(FALSE);
		this->GetDlgItem(IDC_EDIT_HEXWIDTH).EnableWindow(FALSE);
		this->ctlHexUpDown.EnableWindow(FALSE);
	}

	// Check origin mode
	if(BST_CHECKED == this->ctlEnableOriginCheck.GetCheck())
	{
		this->GetDlgItem(IDC_STATIC_ORIGIN_ADR).EnableWindow(TRUE);
		this->ctlOriginAddressEdit.EnableWindow(TRUE);
	}
	else
	{
		this->GetDlgItem(IDC_STATIC_ORIGIN_ADR).EnableWindow(FALSE);
		this->ctlOriginAddressEdit.EnableWindow(FALSE);
	}

	// Check range
	if(BST_CHECKED == this->ctlEnableRangeCheck.GetCheck())
	{
		this->GetDlgItem(IDC_STATIC_START).EnableWindow(TRUE);
		this->ctlBeginAddressEdit.EnableWindow(TRUE);
		this->GetDlgItem(IDC_STATIC_END).EnableWindow(TRUE);
		this->ctlEndAddressEdit.EnableWindow(TRUE);
	}
	else
	{
		this->GetDlgItem(IDC_STATIC_START).EnableWindow(FALSE);
		this->ctlBeginAddressEdit.EnableWindow(FALSE);
		this->GetDlgItem(IDC_STATIC_END).EnableWindow(FALSE);
		this->ctlEndAddressEdit.EnableWindow(FALSE);
	}

}

UINT CMainDlg::Validata()
{
	// check input-output
	CString input;
	CString output;
	this->ctlInputFileEdit.GetWindowText(input);
	this->ctlOutputFileEdit.GetWindowText(output);
	if(0 == input.Compare(output))
	{
		return PERRY_SAMENAME_INOUT;
	}

	// check begin-end
	if(BST_UNCHECKED != this->ctlEnableRangeCheck.GetCheck())
	{
		CString begStr;
		CString endStr;
		this->ctlBeginAddressEdit.GetWindowText(begStr);
		this->ctlEndAddressEdit.GetWindowText(endStr);
		UINT begin = Utils::AsciiToHex(begStr.GetString());
		if(begin >= Utils::AsciiToHex(endStr.GetString()))
		{
			return PERRY_INVERT_BEG_END;
		}
	}

	// OK
	return 0;
}

void CMainDlg::ApplyConfig()
{	
	// 設定値の適用
	this->cfg.hexWidth = this->ctlHexUpDown.GetPos32();
	this->ctlInputFileEdit.GetWindowText(this->cfg.inputPath);
	this->ctlOutputFileEdit.GetWindowText(this->cfg.outputPath);
	this->ctlDisassembleBankEdit.GetWindowText(this->cfg.disassembleBank);
	this->ctlOriginAddressEdit.GetWindowText(this->cfg.originAddress);
	this->ctlBeginAddressEdit.GetWindowText(this->cfg.beginAddress);
	this->ctlEndAddressEdit.GetWindowText(this->cfg.endAddress);
	this->appConfig->Apply(this->cfg);
	if(BST_UNCHECKED != this->ctlBankAutoRadio.GetCheck()) cfg.bankMode = BankMode_Auto;
	else if(BST_UNCHECKED != this->ctlBankLoRomRadio.GetCheck()) cfg.bankMode = BankMode_LoRom;
	else if(BST_UNCHECKED != this->ctlBankHiRomRadio.GetCheck()) cfg.bankMode = BankMode_HiRom;
	if(BST_UNCHECKED != this->ctlRomAddressAutoRadio.GetCheck()) cfg.shadowMode = ShadowAddress_Auto;
	if(BST_UNCHECKED != this->ctlRomAddressEnableRadio.GetCheck()) cfg.shadowMode = ShadowAddress_Enable;
	if(BST_UNCHECKED != this->ctlRomAddressDisableRadio.GetCheck()) cfg.shadowMode = ShadowAddress_Disable;
}