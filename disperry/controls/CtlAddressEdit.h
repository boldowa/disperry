///////////////////////////////////////////////////////////////////////////////
// CtlAddressEdit.h  : アドレス入力コントロール のインターフェース
//
#pragma once

namespace Controls {
	class CCtlAddressEdit : public CWindowImpl<CCtlAddressEdit>
	{
	private:
		int limit;

		bool IsHexInput(TCHAR ch);
	public:
		void SetLimit(int);

		// メッセージマップの定義
		BEGIN_MSG_MAP(CCtlAddressEdit)
			//MESSAGE_HANDLER(msg, func)
			//COMMAND_ID_HANDLER(id, func)
			//NOTIFY_ID_HANDLER(id, func)
			MESSAGE_HANDLER(WM_CHAR, OnInputChar);
		END_MSG_MAP()

		// ハンドラーのプロトタイプ （引数が必要な場合はコメントを外してください）:
		//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		LRESULT OnInputChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	};
}