// disperry.cpp : disperry のメインソースコード
//

#include "stdafx.h"

#include "resource.h"

#include "Puts.h"
#include "ResMan.h"
#include "Enviroment.h"
#include "AppConfig.h"
#include "aboutdlg.h"
#include "MainDlg.h"

CAppModule _Module;

static bool FindDispel(void)
{
	WIN32_FIND_DATA w32fd;
	CPath dispelPath;
	static const TCHAR* SearchPath[] = {
		NULL,		// for exe path
		_T(".\\"),
		NULL		// termination
	};

	// Set exe path
	SearchPath[0] = Enviroment::getExeDir();
	
	// Search dispel
	for(int i=0; SearchPath[i] != NULL; i++)
	{
		dispelPath = SearchPath[i];
		dispelPath += _T("dispel.exe");
		if(INVALID_HANDLE_VALUE != ::FindFirstFile(dispelPath, &w32fd))
		{
			Enviroment::setDispelPath(dispelPath);
			return true;
		}
	}
	return false;
}

static CPath getExeDir(void)
{
	TCHAR buf[MAX_PATH];

	::GetModuleFileName(NULL, buf, MAX_PATH);
	CPath x = buf;
	x.RemoveFileSpec();
	x.m_strPath.Append(_T("\\"));
	return x;
	return (CPath(buf).RemoveFileSpec() + _T("\\"));
}

int Run(CAppConfig& appConfig, LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("メインダイアログの作成に失敗しました！\n"));
		return 0;
	}

	dlgMain.Init(&appConfig);
	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

//------------------------------------------------------------------------------
// アプリケーションの初期化を行います
//   アプリケーションが起動可能な場合はtrueを、
//   起動できない場合(初期化失敗)はfalseを返します
static bool InitApplication(CAppConfig& appConf)
{
	// Init resource manager
	ResMan::SetInstanceHandle(::GetModuleHandle(NULL));

	// Dispel.exe が見つからない場合は起動しません
	if(!FindDispel())
	{
		Disperry::PutFatal(NULL, PERRY_DISPEL_NOT_FOUND);
		return false;
	}

	// Iniファイルの読み出しができない場合は、ユーザーに起動判断を委ねます
	if(false == appConf.LoadIni())
	{
		Disperry::PutWinAPIError(NULL, _T(__func__), _T("ReadPrivateProfileString"));
		if(IDNO == MessageBox(NULL, ResMan::sgetf(PERRY_INI_LOAD_FAIL), ResMan::sgetf(PERRY_WARNING), MB_YESNO|MB_ICONWARNING))
		{
			return false;
		}
		// 初期値に戻します
		appConf.InitConfig();
	}

	return true;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	//もしNT 4.0以上の場合は上の1行を以下のコードで代用できます。
	//HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	//ユニコードレイヤー（MSLU: Windows9x用）使用時のATLウィンドウのサンキング問題を解消します。
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// ほかのコントロール用のフラグを追加してください

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	CPath edir = getExeDir();
	Enviroment::setExeDir(edir);

	// 初期化失敗時は起動しません
	CAppConfig appConf;
	if(!InitApplication(appConf))
	{
		return -1;
	}

	int nRet = Run(appConf, lpstrCmdLine, nCmdShow);

	// 設定を保存します
	if(false == appConf.SaveIni())
	{
		Disperry::PutWinAPIError(NULL, _T(__func__), _T("WritePrivateProfileString"));
		Disperry::PutError(NULL, PERRY_INI_WRITE_FAIL);
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
