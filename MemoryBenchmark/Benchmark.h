#pragma once
#include "BenchmarkResultsLogger.h"
#include <stdio.h>
#include <atomic>
#include <pthread.h>
#include <thread>
#include "Observable.h"
struct BenchmarkParameters {
	BenchmarkParameters() {
		logFileName = std::string("BenchmarkLogs.csv");
	}
	int BlockSize = 128;
	long long RepeatCounter = 1000000;
	std::string logFileName;
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
	void BeginBenchmark();
	char* CreateBuffer(int length);
	void DeleteBuffer();
	long long bytesWritten;
	BenchmarkParameters parameters;
	std::string testFileName;
	std::atomic<bool> interruptionFlag;
	char* buffer;
};


