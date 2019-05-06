#pragma once
#include "BenchmarkResultsLogger.h"
#include <stdio.h>
#include <atomic>
#include <pthread.h>
#include <thread>
#include "Observable.h"
struct BenchmarkParameters {
	BenchmarkParameters() {
		logFilePrefix = std::string("BenchmarkLogs");
	}
	int BlockSize = 128;
	long long RepeatCounter = 1000000;
	std::string logFilePrefix;
};
class Benchmark : public Observable
{
public:
	Benchmark(BenchmarkParameters parameters);
	void Start();
	void Stop();
	~Benchmark();
private:
	std::thread benchmarkThread;
	void BeginWriteBenchmark();
	void BeginReadBenchmark();
	char* CreateBuffer(int length);
	void DeleteBuffer();
	long long bytesWritten = 0;
	long long bytesRead = 0;
	BenchmarkParameters parameters;
	std::string testFileName;
	std::atomic<bool> interruptionFlag;
	char* buffer;
};


