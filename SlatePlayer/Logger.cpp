#include "Logger.h"
#include <stdlib.h>
#include <iostream>
Logger* Logger::Instance = nullptr;

Logger::Logger()
{
    if (!Instance)
    {
        Instance = this;
    }
}
Logger::~Logger()
{
}

void Logger::AddLog(const char* fmt)
{
	std::cout << fmt << '\n';
}
