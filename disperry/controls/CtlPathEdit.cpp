///////////////////////////////////////////////////////////////////////////////
// CtlPathEdit.cpp : パス入力コントロール のメインコード
//
#include "stdafx.h"
#include "../resource.h"
#include "CtlPathEdit.h"

namespace Controls {
	//--------------------------------------------------
	// ファイルドロップ時の動作
	LRESULT CCtlPathEdit::OnDragFile(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return this->SetPathEditText((HDROP)wParam);
	}

	//--------------------------------------------------
	// ドロップされたファイルのファイルパスを取得します
	LRESULT CCtlPathEdit::SetPathEditText(HDROP hDrop)
	{
		TCHAR str[MAX_PATH];

		DragQueryFile(hDrop,0,str,sizeof(str));
		SetWindowText(str);
		return TRUE;
	}
}
