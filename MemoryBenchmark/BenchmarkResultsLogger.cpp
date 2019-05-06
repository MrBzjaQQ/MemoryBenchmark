#include "BenchmarkResultsLogger.h"


BenchmarkResultsLogger::BenchmarkResultsLogger()
{
	logFile = std::string("log.csv");
	SetBytesWritten(0);
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
	logFileHeader = std::string("load,time,speed\n");
	loggerThread = std::thread([this] {BeginWriteLogger(); BeginReadLogger(); });
}

void BenchmarkResultsLogger::Stop()
{
	SetWriteStopped(true);
	SetReadStopped(true);
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
	while (!GetWriteStopped())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		time_t timeBegin = time(NULL);
		system("grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage}' > result");
		std::string load = ReadSystemAnswer();
		load = load.substr(0, load.length() - 2);
		tm *tmStruct = localtime(&timeBegin);
		std::string time;
		time.append(std::to_string(tmStruct->tm_hour)).append(":").append(std::to_string(tmStruct->tm_min)).append(":")
			.append(std::to_string(tmStruct->tm_sec));
		std::string result;
		result.append(load.c_str()).append(",").append(time.c_str()).append(",").append(std::to_string(GetBytesWritten())).append("\n");
		fwrite(result.c_str(), sizeof(char), result.length(), logFileStream);
		SetBytesWritten(0);
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
	while (!GetReadStopped())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		time_t timeBegin = time(NULL);
		system("grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage}' > result");
		std::string load = ReadSystemAnswer();
		load = load.substr(0, load.length() - 2);
		tm *tmStruct = localtime(&timeBegin);
		std::string time;
		time.append(std::to_string(tmStruct->tm_hour)).append(":").append(std::to_string(tmStruct->tm_min)).append(":")
			.append(std::to_string(tmStruct->tm_sec));
		std::string result;
		result.append(load.c_str()).append(",").append(time.c_str()).append(",").append(std::to_string(GetBytesRead())).append("\n");
		fwrite(result.c_str(), sizeof(char), result.length(), logFileStream);
		SetBytesRead(0);
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
