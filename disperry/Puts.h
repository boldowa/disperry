////////////////////////////////////////////////////////////////////////////////
// Puts.cpp : 画面表示ダイアログ インターフェース
//

namespace Disperry {
	void PutDebug(HWND, UINT uID, ...);
	void PutInfo(HWND, UINT uID, ...);
	void PutWarn(HWND, UINT uID, ...);
	void PutError(HWND, UINT uID, ...);
	void PutFatal(HWND, UINT uID, ...);
	void PutWinAPIError(HWND, const TCHAR*, const TCHAR*);
}