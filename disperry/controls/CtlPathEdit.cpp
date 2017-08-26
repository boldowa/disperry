///////////////////////////////////////////////////////////////////////////////
// CtlPathEdit.cpp : �p�X���̓R���g���[�� �̃��C���R�[�h
//
#include "stdafx.h"
#include "../resource.h"
#include "CtlPathEdit.h"

namespace Controls {
	//--------------------------------------------------
	// �t�@�C���h���b�v���̓���
	LRESULT CCtlPathEdit::OnDragFile(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return this->SetPathEditText((HDROP)wParam);
	}

	//--------------------------------------------------
	// �h���b�v���ꂽ�t�@�C���̃t�@�C���p�X���擾���܂�
	LRESULT CCtlPathEdit::SetPathEditText(HDROP hDrop)
	{
		TCHAR str[MAX_PATH];

		DragQueryFile(hDrop,0,str,sizeof(str));
		SetWindowText(str);
		return TRUE;
	}
}
