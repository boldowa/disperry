// MainDlg.h : CMainDlg クラスのインターフェイス
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "controls/CtlAddressEdit.h"
#include "controls/CtlPathEdit.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
private:
	//--- private members ----------------------------
	// application config
	CAppConfig *appConfig;
	AppConf cfg;

	// path edit control
	Controls::CCtlPathEdit ctlInputFileEdit;
	Controls::CCtlPathEdit ctlOutputFileEdit;

	// Spin edit control
	WTL::CUpDownCtrl ctlHexUpDown;

	// rom address edit control
	Controls::CCtlAddressEdit ctlBeginAddressEdit;
	Controls::CCtlAddressEdit ctlEndAddressEdit;
	Controls::CCtlAddressEdit ctlDisassembleBankEdit;
	Controls::CCtlAddressEdit ctlOriginAddressEdit;
	Controls::CCtlAddressEdit ctlHexWidthEdit;

	// checkbox control
	WTL::CButton ctlEnableSkipHeaderCheck;
	WTL::CButton ctlDisableHexDumpCheck;
	WTL::CButton ctlIs8bitsAccumlatorCheck;
	WTL::CButton ctlIs8bitsIndexRegisterCheck;
	WTL::CButton ctlDisableBoundaryCheck;
	WTL::CButton ctlEnableSplitCheck;
	WTL::CButton ctlEnable1BankDisassembleCheck;
	WTL::CButton ctlDisableDisassembleCheck;
	WTL::CButton ctlEnableOriginCheck;
	WTL::CButton ctlEnableRangeCheck;

	// BankMap radiobottun control
	WTL::CButton ctlBankAutoRadio;
	WTL::CButton ctlBankLoRomRadio;
	WTL::CButton ctlBankHiRomRadio;

	// MirrorAddress radiobutton control
	WTL::CButton ctlRomAddressAutoRadio;
	WTL::CButton ctlRomAddressEnableRadio;
	WTL::CButton ctlRomAddressDisableRadio;


	//--- private methodss ---------------------------
	void CloseDialog(int nVal);
	void AssociateControls(void);
	LRESULT GenerateOutputPathFromInputPath(void);
	LRESULT UpdateHexWidthConfig(void);
	LRESULT ChangeBankMapMode(WORD);
	LRESULT ChangeShadowAddressMode(WORD);
	LRESULT ChangeCheckBoxStatus(WORD);
	void UpdateEnableStatus(void);
//	LRESULT SetFilePathEdit(CFileDialog&, Controls::CCtlPathEdit&);
	UINT Validata();
	void ApplyConfig();

public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	void Init(CAppConfig*);

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_DISASM, OnDisassemble)
		COMMAND_ID_HANDLER(IDEXIT, OnExit)
		COMMAND_ID_HANDLER(IDCANCEL, OnExit)					// * for [x] button
		COMMAND_ID_HANDLER(IDC_BUTTON_INPUT, OnSelectInputFile)
		COMMAND_ID_HANDLER(IDC_BUTTON_OUTPUT, OnSelectOutputFile)
		COMMAND_ID_HANDLER(IDC_EDIT_INPUT, OnEditInputPath)
		COMMAND_ID_HANDLER(IDC_EDIT_HEXWIDTH, OnEditHexWidth)
		COMMAND_CODE_HANDLER(BN_CLICKED, OnCheckBoxClicked)
	END_MSG_MAP()

// ハンドラーのプロトタイプ （引数が必要な場合はコメントを外してください）:
//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDisassemble(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSelectInputFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSelectOutputFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditInputPath(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditHexWidth(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCheckBoxClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};