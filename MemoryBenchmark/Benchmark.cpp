#include "Benchmark.h"

Benchmark::Benchmark(BenchmarkParameters parameters)
{
	this->parameters = parameters;
	interruptionFlag.store(false);
	bytesWritten = 0;
}

void Benchmark::Start()
{
	benchmarkThread = std::thread([this] { BeginWriteBenchmark(); BeginReadBenchmark(); });
	benchmarkThread.join();
}

void Benchmark::Stop()
{
	interruptionFlag.store(true);
}

Benchmark::~Benchmark()
{
	
}

void Benchmark::BeginWriteBenchmark()
{
	std::cout << "Write benchmark has started" << std::endl;
	FILE *testFile;
	testFileName = tempnam("/root/Benchmark", "f");
	testFile = fopen(testFileName.c_str(), "w+");
	buffer = CreateBuffer(parameters.BlockSize);
	if (!testFile)
	{
		std::cout << "Cannot create file" << std::endl;
		return;
	}
	for (int iteration = 0; iteration < parameters.RepeatCounter && interruptionFlag.load() == false; iteration++)
	{
		timespec mt1, mt2;
		clock_gettime(CLOCK_MONOTONIC_COARSE, &mt1);
		int bytesWritten = fwrite(buffer, sizeof(char), parameters.BlockSize, testFile);
		fsync(testFile->_fileno);
		clock_gettime(CLOCK_MONOTONIC_COARSE, &mt2);
		long duration = (mt2.tv_sec * 1000000LL + mt2.tv_nsec / 1000) - (mt1.tv_sec * 1000000LL + mt1.tv_nsec / 1000);
		LogRecord record;
		record.amountOfBytes = bytesWritten;
		record.executionTime = duration;
		record.timestamp = time(NULL);
		record.cpuLoad = Procinfo::GetCPULoad();
		NotifyObserversOnWritten(record);
	}
	DeleteBuffer();
	fclose(testFile);
}

void Benchmark::BeginReadBenchmark()
{
	std::cout << "Read benchmark has started" << std::endl;
	std::string newName(tempnam("/root/Benchmark", "f"));
	rename(testFileName.c_str(), newName.c_str());
	testFileName = newName;
	FILE *testFile = fopen(testFileName.c_str(), "r");
	if (!testFile)
	{
		std::cout << "Cannot read file." << std::endl;
		return;
	}
	buffer = new char[parameters.BlockSize];
	for (int iteration = 0; iteration < parameters.RepeatCounter && interruptionFlag.load() == false; iteration++)
	{
		timespec mt1, mt2;
		clock_gettime(CLOCK_MONOTONIC_COARSE, &mt1);
		int bytesRead = fread(buffer, sizeof(char), parameters.BlockSize, testFile);
		clock_gettime(CLOCK_MONOTONIC_COARSE, &mt2);
		long duration = (mt2.tv_sec * 1000000LL + mt2.tv_nsec / 1000) - (mt1.tv_sec * 1000000LL + mt1.tv_nsec / 1000);
		LogRecord record;
		record.amountOfBytes = bytesRead;
		record.executionTime = duration;
		record.timestamp = time(NULL);
		record.cpuLoad = Procinfo::GetCPULoad();
		NotifyObserversOnRead(record);
	}
	bool isFinished = GetFinished();
	NotifyObservers(&isFinished);
	fclose(testFile);
	remove(testFileName.c_str());
	delete[] buffer;
	std::cout << "Benchmark has finished" << std::endl;
	std::cout << "Please find your results in " << parameters.logFilePrefix.c_str()
		<< "-write.csv and " << parameters.logFilePrefix.c_str() << "-read.csv" << std::endl;
}

char * Benchmark::CreateBuffer(int length)
{
	char *buffer = new char[length];
	for (int i = 0; i < length; i++)
		buffer[i] = 'C';
	return buffer;
}

void Benchmark::DeleteBuffer()
{
	if (buffer != nullptr)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}