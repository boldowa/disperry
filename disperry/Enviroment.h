////////////////////////////////////////////////////////////////////////////////
// Enviroment.h : 環境変数 インターフェイス
#pragma once

class Enviroment
{
private:
	// static class
	Enviroment(void){}

	static CPath m_exeDir;
	static CPath m_dispelPath;

public:
	static void setExeDir(const TCHAR*);
	static void setDispelPath(const TCHAR*);
	static const CPath& getExeDir(void);
	static const CPath& getDispelPath(void);
};
