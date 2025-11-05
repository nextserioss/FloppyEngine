#pragma once

#include <string>
#include <Windows.h>

using namespace std;

#define BUFFER_SIZE 128
#define REG_PATH "SOFTWARE\\JabiStudio\\Woogeok"

class RegDataManager{
private:
	static RegDataManager* instance;
	HKEY m_hKey;
public:
	static RegDataManager* GetInstance()
	{
		if (instance == NULL)
		{
			instance = new RegDataManager();
			instance->OpenReg();
			atexit(Destroy);
		}
		return instance;
	}
	static void Destroy()
	{
		instance->CloseReg();
		delete instance;
	}
	void OpenReg();
	DWORD ReadDWORDValue(string strKey);
	void WriteDWORDValue(string strKey, DWORD value);
	void DeleteValue(string strKey);
	void CloseReg();
};