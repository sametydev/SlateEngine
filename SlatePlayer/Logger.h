#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Core/ILogger.h>

class Logger : public ILogger
{
public:
	Logger();
	~Logger();

	void AddLog(const char* fmt) override;

	static Logger* Instance;
};