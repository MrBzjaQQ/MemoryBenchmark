#pragma once
#include <atomic>
#include <list>
#include <condition_variable>
#include "LogRecord.h"
class Observer
{
protected:
	std::list<LogRecord> readRecords;
	std::list<LogRecord> writeRecords;
	std::condition_variable benchmarkCompleted;
public:
	Observer();
	~Observer();
	virtual void Update(bool *argument);
	virtual void UpdateWrite(LogRecord record);
	virtual void UpdateRead(LogRecord record);
};

