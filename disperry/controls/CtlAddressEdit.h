///////////////////////////////////////////////////////////////////////////////
// CtlAddressEdit.h  : �A�h���X���̓R���g���[�� �̃C���^�[�t�F�[�X
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

		// ���b�Z�[�W�}�b�v�̒�`
		BEGIN_MSG_MAP(CCtlAddressEdit)
			//MESSAGE_HANDLER(msg, func)
			//COMMAND_ID_HANDLER(id, func)
			//NOTIFY_ID_HANDLER(id, func)
			MESSAGE_HANDLER(WM_CHAR, OnInputChar);
		END_MSG_MAP()

		// �n���h���[�̃v���g�^�C�v �i�������K�v�ȏꍇ�̓R�����g���O���Ă��������j:
		//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		LRESULT OnInputChar(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	};
}