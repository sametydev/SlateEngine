#pragma once

class ENGINE_API ILogger {
public:
	virtual void AddLog(const char* fmt) = 0;
};