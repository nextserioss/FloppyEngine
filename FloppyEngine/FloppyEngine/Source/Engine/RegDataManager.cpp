#include "../../Header/Engine/RegDataManager.h"
#include "../../Header/Engine/LogManager.h"

RegDataManager* RegDataManager::instance = NULL;

void RegDataManager::OpenReg()
{
	// 해당 경로의 레지스트리를 open 한다.
	// 레지스트리를 성공적으로 open 하였다면 ERROR_SUCCESS값이 reg 에 저장된다.
	LONG lRes = RegOpenKeyEx(HKEY_CURRENT_USER, REG_PATH, 0L, KEY_READ | KEY_WRITE | KEY_QUERY_VALUE, &m_hKey);
	if (ERROR_SUCCESS != lRes)
	{
		// 레지스트리를 열지 못했다면 새로 생성한다.
		lRes = RegCreateKeyEx(HKEY_CURRENT_USER, REG_PATH, 0, "", 0, KEY_READ | KEY_WRITE | KEY_QUERY_VALUE, NULL, &m_hKey, NULL);
		if (ERROR_SUCCESS != lRes)
		{
			DLOG("Reg Error %d", lRes);
			return;
		}
	}
}

//데이터가 없으면 0 리턴
DWORD RegDataManager::ReadDWORDValue(string strKey)
{
	char valueArray[BUFFER_SIZE];
	DWORD valueLength = sizeof(valueArray);
	DWORD dwActualType;
	DWORD error = RegQueryValueEx(m_hKey, strKey.c_str(), NULL, &dwActualType, (LPBYTE)&valueArray, &valueLength);
	if (error == ERROR_SUCCESS)
	{
		if (dwActualType == REG_DWORD)
		{
			DWORD nValue;
			memcpy(&nValue, valueArray, valueLength);
			DLOG("Reg Key type %d value is: %d size: %d", dwActualType, nValue, valueLength);
			return nValue;
		}
		else
		{
			DLOG("Reg Key type %d is not DWORD", dwActualType);
			return 0;
		}
	}
	else
	{
		DLOG(" Reg Error is: %d", error);
		return 0;
	}
}

void RegDataManager::WriteDWORDValue(string strKey, DWORD value)
{
	LONG lRes = RegSetValueEx(m_hKey, strKey.c_str(), 0, REG_DWORD, (BYTE *)&value, sizeof(DWORD));
	if (lRes != ERROR_SUCCESS)
	{
		DLOG("Reg Fail %d", lRes);
	}
}

void RegDataManager::DeleteValue(string strKey)
{
	LONG lRes = RegDeleteValue(m_hKey, strKey.c_str());
	if (lRes != ERROR_SUCCESS)
	{
		DLOG("Reg Delete Fail %d", lRes);
	}
}

void RegDataManager::CloseReg()
{
	RegCloseKey(m_hKey);
}

