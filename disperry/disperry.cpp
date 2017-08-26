// disperry.cpp : disperry �̃��C���\�[�X�R�[�h
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
		ATLTRACE(_T("���C���_�C�A���O�̍쐬�Ɏ��s���܂����I\n"));
		return 0;
	}

	dlgMain.Init(&appConfig);
	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

//------------------------------------------------------------------------------
// �A�v���P�[�V�����̏��������s���܂�
//   �A�v���P�[�V�������N���\�ȏꍇ��true���A
//   �N���ł��Ȃ��ꍇ(���������s)��false��Ԃ��܂�
static bool InitApplication(CAppConfig& appConf)
{
	// Init resource manager
	ResMan::SetInstanceHandle(::GetModuleHandle(NULL));

	// Dispel.exe ��������Ȃ��ꍇ�͋N�����܂���
	if(!FindDispel())
	{
		Disperry::PutFatal(NULL, PERRY_DISPEL_NOT_FOUND);
		return false;
	}

	// Ini�t�@�C���̓ǂݏo�����ł��Ȃ��ꍇ�́A���[�U�[�ɋN�����f���ς˂܂�
	if(false == appConf.LoadIni())
	{
		Disperry::PutWinAPIError(NULL, _T(__func__), _T("ReadPrivateProfileString"));
		if(IDNO == MessageBox(NULL, ResMan::sgetf(PERRY_INI_LOAD_FAIL), ResMan::sgetf(PERRY_WARNING), MB_YESNO|MB_ICONWARNING))
		{
			return false;
		}
		// �����l�ɖ߂��܂�
		appConf.InitConfig();
	}

	return true;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	//����NT 4.0�ȏ�̏ꍇ�͏��1�s���ȉ��̃R�[�h�ő�p�ł��܂��B
	//HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	//���j�R�[�h���C���[�iMSLU: Windows9x�p�j�g�p����ATL�E�B���h�E�̃T���L���O�����������܂��B
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// �ق��̃R���g���[���p�̃t���O��ǉ����Ă�������

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	CPath edir = getExeDir();
	Enviroment::setExeDir(edir);

	// ���������s���͋N�����܂���
	CAppConfig appConf;
	if(!InitApplication(appConf))
	{
		return -1;
	}

	int nRet = Run(appConf, lpstrCmdLine, nCmdShow);

	// �ݒ��ۑ����܂�
	if(false == appConf.SaveIni())
	{
		Disperry::PutWinAPIError(NULL, _T(__func__), _T("WritePrivateProfileString"));
		Disperry::PutError(NULL, PERRY_INI_WRITE_FAIL);
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
