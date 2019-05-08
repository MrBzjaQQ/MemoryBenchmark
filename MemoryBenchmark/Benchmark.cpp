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
	testFileName = std::string("testFile");
	testFile = fopen(testFileName.c_str(), "w+");
	buffer = CreateBuffer(parameters.BlockSize);
	if (!testFile)
	{
		std::cout << "Cannot create file" << std::endl;
		return;
	}
	for (int iteration = 0; iteration < parameters.RepeatCounter && interruptionFlag.load() == false; iteration++)
	{
		//int handle = fileno(testFile);
		clock_t start = clock();
		int bytesWritten = fwrite(buffer, sizeof(char), parameters.BlockSize, testFile);
		clock_t finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
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
	FILE *testFile = fopen(testFileName.c_str(), "r");
	if (!testFile)
	{
		std::cout << "Cannot read file." << std::endl;
		return;
	}
	buffer = new char[parameters.BlockSize];
	for (int iteration = 0; iteration < parameters.RepeatCounter && interruptionFlag.load() == false; iteration++)
	{
		clock_t start = clock();
		int bytesRead = fread(buffer, sizeof(char), parameters.BlockSize, testFile);
		clock_t finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
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
	remove("testFile");
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