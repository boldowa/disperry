///////////////////////////////////////////////////////////////////////////////
// CtlAddresEdit.cpp : アドレス入力コントロール のメインコード
//
#include "stdafx.h"
#include "../Utils.h"
#include "../resource.h"
#include "CtlAddressEdit.h"

namespace Controls {
	void CCtlAddressEdit::SetLimit(int limit)
	{
		this->limit = limit;
	}

	LRESULT CCtlAddressEdit::OnInputChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(!Utils::IsHex(wParam))
		{
			switch(wParam)
			{
			case VK_BACK:
			case VK_DELETE:
				return bHandled = FALSE;
			default:
				break;
			}
			return 0;
		}

		// limit
		CString str;
		this->GetWindowText(str);
		if(this->limit <= str.GetLength()) return 0;

		bHandled = FALSE;
		return 0;
	}

	bool CCtlAddressEdit::IsHexInput(TCHAR ch)
	{
		if(_istdigit(ch))
		{
			return true;
		}

		ch = _totupper(ch);
		if(_T('A') <= ch && _T('F') >= ch)
		{
			return true;
		}

		return false;
	}
}
