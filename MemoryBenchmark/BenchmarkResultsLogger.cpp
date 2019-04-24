#include "BenchmarkResultsLogger.h"


BenchmarkResultsLogger::BenchmarkResultsLogger()
{
	logFile = std::string("log.csv");
	logFileHeader = std::string("load,time,speed\n");
	SetBytesWritten(0);
}

BenchmarkResultsLogger::BenchmarkResultsLogger(std::string logFilename)
{
	logFile = logFilename;
	BenchmarkResultsLogger();
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
	loggerThread = std::thread([this] {BeginLogger(); });
}

void BenchmarkResultsLogger::Stop()
{
	SetStopped(true);
}

void BenchmarkResultsLogger::BeginLogger()
{
	logFileStream = fopen(logFile.c_str(), "w+");
	if (!logFileStream)
	{
		std::cout << "Can't open file " << logFile.c_str() << std::endl;
		return;
	}
	fwrite(logFileHeader.c_str(), sizeof(char), logFileHeader.length(), logFileStream);
	while (!GetStopped())
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
	workFinished = true;
}

std::string BenchmarkResultsLogger::ReadSystemAnswer()
{
	FILE* resultFile = fopen("result", "r");
	if (!resultFile)
	{
		std::cout << "Can't read file with result infformation" << std::endl;
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
