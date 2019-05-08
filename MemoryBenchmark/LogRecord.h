#pragma once
#include <ctime>
#include <string>
struct LogRecord
{
	time_t timestamp;
	double executionTime;
	double cpuLoad;
	long long amountOfBytes;
public:
	std::string ToString();
	std::string ToStringWithSpeed();
	LogRecord();
	~LogRecord();
};

