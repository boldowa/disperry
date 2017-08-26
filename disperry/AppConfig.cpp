#include "StdAfx.h"
#include "Enviroment.h"
#include "AppConfig.h"

const TCHAR* CAppConfig::iniFileName = _T("disperry.ini");

const TCHAR* CAppConfig::secPosition = _T("Position");
const TCHAR* CAppConfig::secFile     = _T("File");
const TCHAR* CAppConfig::secDispel   = _T("Dispel");
CAppConfigDefines_KEYDEFS(CAppConfig);

enum DataType {
	StrData,
	IntData,
	BoolData
};

CAppConfig::CAppConfig(void)
{
	this->iniFilePath += Enviroment::getExeDir() + iniFileName;
	this->InitConfig();
}

CAppConfig::~CAppConfig(void)
{
}

void CAppConfig::InitConfig(void)
{
	CAppConfigDefines_INIT();
}

bool CAppConfig::LoadIni(void)
{
	// [Position]
	if(!this->ReadPrivateProfile(secPosition, KeyPosX, IntData, &this->posX)) return false;
	if(!this->ReadPrivateProfile(secPosition, KeyPosY, IntData, &this->posY)) return false;
	// [File]
	if(!this->ReadPrivateProfile(secFile, KeyInputPath, StrData, &this->inputPath)) return false;
	if(!this->ReadPrivateProfile(secFile, KeyOutputPath, StrData, &this->outputPath)) return false;
	// [Dispel]
	if(!this->ReadPrivateProfile(secDispel, KeyEnableSkipHeader, BoolData, &this->enableSkipHeader)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyDisableHexDump, BoolData, &this->disableHexDump)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyIs8bitsAccumlator, BoolData, &this->is8bitsAccumlator)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyIs8bitsIndexRegister, BoolData, &this->is8bitsIndexRegister)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyDisableBoundary, BoolData, &this->disableBoundary)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyEnableSplit, BoolData, &this->enableSplit)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyEnable1BankDisassemble, BoolData, &this->enable1BankDisassemble)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyDisableDisassemble, BoolData, &this->disableDisassemble)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyEnableOrigin, BoolData, &this->enableOrigin)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyEnableRange, BoolData, &this->enableRange)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyHexWidth, IntData, &this->hexWidth)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyDisassembleBank, StrData, &this->disassembleBank)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyOriginAddress, StrData, &this->originAddress)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyBeginAddress, StrData, &this->beginAddress)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyEndAddress, StrData, &this->endAddress)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyBankMode, IntData, &this->bankMode)) return false;
	if(!this->ReadPrivateProfile(secDispel, KeyShadowMode, IntData, &this->shadowMode)) return false;

	return true;
}

AppConf CAppConfig::Get(void)
{
	CAppConfigDefines_GET(AppConf);
}

bool CAppConfig::Apply(AppConf cfg)
{
	CAppConfigDefines_APPLY(cfg);
	return true;
}

bool CAppConfig::SaveIni(void)
{
	// [Position]
	if(!this->WritePrivateProfile(secPosition, KeyPosX, IntData, &this->posX)) return false;
	if(!this->WritePrivateProfile(secPosition, KeyPosY, IntData, &this->posY)) return false;
	// [File]
	if(!this->WritePrivateProfile(secFile, KeyInputPath, StrData, &this->inputPath)) return false;
	if(!this->WritePrivateProfile(secFile, KeyOutputPath, StrData, &this->outputPath)) return false;
	// [Dispel]
	if(!this->WritePrivateProfile(secDispel, KeyEnableSkipHeader, BoolData, &this->enableSkipHeader)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyDisableHexDump, BoolData, &this->disableHexDump)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyIs8bitsAccumlator, BoolData, &this->is8bitsAccumlator)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyIs8bitsIndexRegister, BoolData, &this->is8bitsIndexRegister)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyDisableBoundary, BoolData, &this->disableBoundary)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyEnableSplit, BoolData, &this->enableSplit)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyEnable1BankDisassemble, BoolData, &this->enable1BankDisassemble)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyDisableDisassemble, BoolData, &this->disableDisassemble)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyEnableOrigin, BoolData, &this->enableOrigin)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyEnableRange, BoolData, &this->enableRange)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyHexWidth, IntData, &this->hexWidth)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyDisassembleBank, StrData, &this->disassembleBank)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyOriginAddress, StrData, &this->originAddress)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyBeginAddress, StrData, &this->beginAddress)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyEndAddress, StrData, &this->endAddress)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyBankMode, IntData, &this->bankMode)) return false;
	if(!this->WritePrivateProfile(secDispel, KeyShadowMode, IntData, &this->shadowMode)) return false;

	return true;
}

BOOL CAppConfig::ReadPrivateProfile(const TCHAR* sec, const TCHAR* key, int type, void* vRead)
{
	TCHAR buf[512];

	if(!::GetPrivateProfileString(sec, key, _T(""), buf, 512, this->iniFilePath))
	{
		switch(::GetLastError())
		{
		case 0:	/* non-error */
				break;

		case 2: /* file not found */
			return TRUE;

		default: /* other errors */
			return FALSE;
		}
	}

	switch(type)
	{
	case StrData:
		{
			CString* s = (CString*)vRead;
			(*s) = buf;
			return TRUE;
		}
	case IntData:
		{
			int* i = (int*)vRead;
			(*i) = _ttoi(buf);
			return TRUE;
		}
	case BoolData:
		{
			bool* b = (bool*)vRead;
			(*b) = (_T('0') != buf[0]);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CAppConfig::WritePrivateProfile(const TCHAR* sec, const TCHAR* key, int type, void* vWrite)
{
	const TCHAR* str;
	CString conv;
	switch(type)
	{
	case StrData:
		{
		CString *cv = (CString*)vWrite;
		str = cv->GetString();
		break;
		}

	case BoolData:
		{
		bool *b = (bool*)vWrite;
		conv.Format(_T("%d"), (*b) ? 1 : 0);
		str = conv.GetString();
		break;
		}

	case IntData:
		{
		int *i = (int*)vWrite;
		conv.Format(_T("%d"), (*i));
		str = conv.GetString();
		break;
		}
	}
	return ::WritePrivateProfileString(sec, key, str, this->iniFilePath);
}