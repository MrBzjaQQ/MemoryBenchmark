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
		fwrite(buffer, sizeof(buffer), parameters.BlockSize, testFile);
		bytesWritten += parameters.BlockSize;
		if (iteration % 10000 == 9999)
		{
			NotifyObserversOnWritten(&bytesWritten);
			bytesWritten = 0;
		}
	}
	SetWriteFinished(true);
	DeleteBuffer();
	fclose(testFile);
}

void Benchmark::BeginReadBenchmark()
{
	FILE *testFile = fopen(testFileName.c_str(), "w+");
	if (!testFile)
	{
		std::cout << "Cannot read file." << std::endl;
		return;
	}
	buffer = new char[parameters.BlockSize];
	for (int iteration = 0; iteration < parameters.RepeatCounter && interruptionFlag.load() == false; iteration++)
	{
		fread(buffer, sizeof(buffer), parameters.BlockSize, testFile);
		bytesRead += parameters.BlockSize;
		if (iteration % 10000 == 9999)
		{
			NotifyObserversOnRead(&bytesRead);
			bytesRead = 0;
		}
	}
	SetReadFinished(true);
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