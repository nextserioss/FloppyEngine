#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

class LogManager
{
private:	
	LogManager();
	~LogManager(){};
	static LogManager* instance;
	HANDLE m_ConsoleHandler;
public:
	
	static LogManager* GetInstance()
	{
		if (instance == NULL)
		{
			instance = new LogManager();
			atexit(Destroy);
		}
		return instance;
	}
	static void Destroy()
	{
		delete instance;
	}
	void DLog(const char * format, ...);
};

#define DLOG LogManager::GetInstance()->DLog