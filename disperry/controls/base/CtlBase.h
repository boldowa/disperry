///////////////////////////////////////////////////////////////////////////////
// CtlBase.h  : ***�R���g���[�� �̃C���^�[�t�F�[�X
//
#pragma once

namespace Controls {
	class CCtlBase : public CWindowImpl<CCtlBase>
	{
	public:
		// ���b�Z�[�W�}�b�v�̒�`
		BEGIN_MSG_MAP(CCtlBase)
			//MESSAGE_HANDLER(msg, func)
			//COMMAND_ID_HANDLER(id, func)
			//NOTIFY_ID_HANDLER(id, func)
		END_MSG_MAP()

		// �n���h���[�̃v���g�^�C�v �i�������K�v�ȏꍇ�̓R�����g���O���Ă��������j:
		//LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	};
}