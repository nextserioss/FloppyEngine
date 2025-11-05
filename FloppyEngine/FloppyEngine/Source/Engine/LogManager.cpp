#include "../../Header/Engine/LogManager.h"
#include "../../Header/Engine/GameEngine.h"

LogManager* LogManager::instance = NULL;

LogManager::LogManager()
{
	m_ConsoleHandler = GetStdHandle(STD_OUTPUT_HANDLE);
}

void LogManager::DLog(const char * format, ...)
{	
#ifdef _DEBUG
	va_list args;

	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);
#endif
}
