///////////////////////////////////////////////////////////////////////////////
// CtlPathEdit.h  : �p�X���̓R���g���[�� �̃C���^�[�t�F�[�X
//
#pragma once

namespace Controls
{
	class CCtlPathEdit : public CWindowImpl<CCtlPathEdit>
	{
	private:
		LRESULT SetPathEditText(HDROP);
	public:
		// ���b�Z�[�W�}�b�v�̒�`
		BEGIN_MSG_MAP(CCtlPathEdit)
			//MESSAGE_HANDLER(msg, func)
			//COMMAND_ID_HANDLER(id, func)
			//NOTIFY_ID_HANDLER(id, func)
			MESSAGE_HANDLER(WM_DROPFILES,OnDragFile)
		END_MSG_MAP()

		// �n���h���[�̃v���g�^�C�v �i�������K�v�ȏꍇ�̓R�����g���O���Ă��������j:
		//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		LRESULT OnDragFile(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	};
}