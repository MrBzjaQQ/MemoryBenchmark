#include "BenchmarkResultsLogger.h"


BenchmarkResultsLogger::BenchmarkResultsLogger()
{
	logFile = std::string("log.csv");
}

BenchmarkResultsLogger::BenchmarkResultsLogger(std::string logFilename)
{
	BenchmarkResultsLogger();
	logFile = logFilename;
}


BenchmarkResultsLogger::~BenchmarkResultsLogger()
{

}

bool BenchmarkResultsLogger::GetWorkFinished()
{
	return workFinished;
}

void BenchmarkResultsLogger::JoinThread()
{
	loggerThread.join();
}

void BenchmarkResultsLogger::Start()
{
	logFileHeader = std::string("local_time (hh:mm:ss);execution_time (s);cpu_load;amount_of_bytes;speed (bytes/s)\n");
	/*
		time_t timestamp;
	double executionTime;
	double cpuLoad;
	long long amountOfBytes;
	*/
	loggerThread = std::thread([this] {BeginWriteLogger(); BeginReadLogger(); });
}

void BenchmarkResultsLogger::BeginWriteLogger()
{
	std::string writeLog = std::string(logFile);
	logFileStream = fopen(writeLog.append("-write.csv").c_str(), "w+");
	if (!logFileStream)
	{
		std::cout << "Can't open file " << logFile.c_str() << std::endl;
		return;
	}
	fwrite(logFileHeader.c_str(), sizeof(char), logFileHeader.length(), logFileStream);
	for (LogRecord rec : writeRecords) {
		std::string record = rec.ToStringWithSpeed();
		fwrite(record.c_str(), sizeof(char), record.length(), logFileStream);
	}
	fclose(logFileStream);
}

void BenchmarkResultsLogger::BeginReadLogger()
{
	std::cout << "Read benchmark has started" << std::endl;
	std::string readLog = std::string(logFile);
	logFileStream = fopen(readLog.append("-read.csv").c_str(), "w+");
	if (!logFileStream)
	{
		std::cout << "Can't open file " << logFile.c_str() << std::endl;
		return;
	}
	fwrite(logFileHeader.c_str(), sizeof(char), logFileHeader.length(), logFileStream);
	for (LogRecord rec : readRecords) {
		std::string record = rec.ToStringWithSpeed();
		fwrite(record.c_str(), sizeof(char), record.length(), logFileStream);
	}
	fclose(logFileStream);
	workFinished = true;
}

std::string BenchmarkResultsLogger::ReadSystemAnswer()
{
	FILE* resultFile = fopen("result", "r");
	if (!resultFile)
	{
		std::cout << "Can't read file with result information" << std::endl;
		return std::string("");
	}
	char result[100];
	fread(result, sizeof(char), 99, resultFile);
	fclose(resultFile);
	remove("result");
	return std::string(result);
}

std::string BenchmarkResultsLogger::ExecuteConsoleCommand(std::string command)
{
	return std::string();
}
