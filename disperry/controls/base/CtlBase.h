///////////////////////////////////////////////////////////////////////////////
// CtlBase.h  : ***コントロール のインターフェース
//
#pragma once

namespace Controls {
	class CCtlBase : public CWindowImpl<CCtlBase>
	{
	public:
		// メッセージマップの定義
		BEGIN_MSG_MAP(CCtlBase)
			//MESSAGE_HANDLER(msg, func)
			//COMMAND_ID_HANDLER(id, func)
			//NOTIFY_ID_HANDLER(id, func)
		END_MSG_MAP()

		// ハンドラーのプロトタイプ （引数が必要な場合はコメントを外してください）:
		//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	};
}