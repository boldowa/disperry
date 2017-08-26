////////////////////////////////////////////////////////////////////////////////
// Enviroment.h : ä¬ã´ïœêî
#include "StdAfx.h"
#include "Enviroment.h"

// static members
CPath Enviroment::m_exeDir;
CPath Enviroment::m_dispelPath;

void Enviroment::setExeDir(const TCHAR* exeDir)
{
	m_exeDir = exeDir;
}
void Enviroment::setDispelPath(const TCHAR* dispelPath)
{
	m_dispelPath = dispelPath;
}
const CPath& Enviroment::getExeDir(void)
{
	return m_exeDir;
}
const CPath& Enviroment::getDispelPath(void)
{
	return m_dispelPath;
}