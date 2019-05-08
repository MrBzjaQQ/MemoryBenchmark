#pragma once
#include <string>
#include <thread>
#include <iostream>
#include <string>
#include "Observer.h"
#include <stdio.h>
#include <ctime>
#include <list>
#include "procinfo.h"
#include "LogRecord.h"
//пишет логи в .csv в отдельном потоке.
class BenchmarkResultsLogger : public Observer
{
public:
	BenchmarkResultsLogger();
	BenchmarkResultsLogger(std::string logFilename);
	~BenchmarkResultsLogger();
	bool GetWorkFinished();
	void JoinThread();
	void Start();
private:
	bool workFinished;
	std::string logFileHeader;
	std::string logFile;
	std::string systemAnswer;
	FILE *logFileStream;
	std::thread loggerThread;
	void BeginWriteLogger();
	void BeginReadLogger();
	std::string ReadSystemAnswer();
	std::string ExecuteConsoleCommand(std::string command);
};

