#pragma once

class ILogger {
public:
	virtual void AddLog(const char* fmt) = 0;
};