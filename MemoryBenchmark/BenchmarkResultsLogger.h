#pragma once
#include <string>
#include <thread>
#include <iostream>
#include <string>
#include "Observer.h"
#include <stdio.h>
#include <ctime>
#include <list>
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
	void Stop();
private:
	//std::list<std::tuple<double, std::string, long long>> readRecords; //список записей с результатами чтения
	//std::list<std::tuple<double, std::string, long long>> writeRecords; //список записей с результатами записи
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

