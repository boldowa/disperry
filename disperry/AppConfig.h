#pragma once
#include "AppConfigDefines.h"

typedef enum _BankMode {
	BankMode_Auto,
	BankMode_LoRom,
	BankMode_HiRom
} BankMode;

typedef enum _ShadowAddressMode {
	ShadowAddress_Auto,
	ShadowAddress_Enable,
	ShadowAddress_Disable,
} ShadowAddressMode;

typedef struct _AppConf {
	CAppConfigDefines_DECLARE();
}AppConf;

class CAppConfig
{
private:
	// ini file name
	static const TCHAR* iniFileName;

	// section name
	static const TCHAR* secPosition;
	static const TCHAR* secFile;
	static const TCHAR* secDispel;

	CPath iniFilePath;

	// key
	CAppConfigDefines_KEYDECS();

	// disperry configs
	CAppConfigDefines_DECLARE();

	// Config load-write function
	BOOL ReadPrivateProfile(const TCHAR*, const TCHAR*, int, void*);
	BOOL WritePrivateProfile(const TCHAR*, const TCHAR*, int, void*);

public:
	CAppConfig(void);
	~CAppConfig(void);
	void InitConfig(void);
	bool LoadIni(void);
	AppConf Get(void);
	bool Apply(AppConf);
	bool SaveIni(void);
};
