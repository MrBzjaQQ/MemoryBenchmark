#include "LogRecord.h"



std::string LogRecord::ToString()
{
	std::string result;
	std::string time;
	tm *_tm = localtime(&timestamp);
	time.append(std::to_string(_tm->tm_hour)).append(":").append(std::to_string(_tm->tm_min)).append(":").append(std::to_string(_tm->tm_sec));
	result.append(time).append(";").append(std::to_string(executionTime)).append(";").append(std::to_string(cpuLoad)).append(";").append(std::to_string(amountOfBytes)).append("\n");
	return result;
}

std::string LogRecord::ToStringWithSpeed()
{
	std::string result;
	std::string time;
	tm *_tm = localtime(&timestamp);
	time.append(std::to_string(_tm->tm_hour)).append(":").append(std::to_string(_tm->tm_min)).append(":").append(std::to_string(_tm->tm_sec));
	result.append(time).append(";").append(std::to_string(executionTime)).append(";").append(std::to_string(cpuLoad)).append(";").append(std::to_string(amountOfBytes)).append(";").append(std::to_string(amountOfBytes/executionTime)).append("\n");
	return result;
}

LogRecord::LogRecord()
{
}


LogRecord::~LogRecord()
{
}
