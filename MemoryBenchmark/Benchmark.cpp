#include "Benchmark.h"

Benchmark::Benchmark(BenchmarkParameters parameters)
{
	this->parameters = parameters;
	interruptionFlag.store(false);
	bytesWritten = 0;
}

void Benchmark::Start()
{
	benchmarkThread = std::thread([this] { BeginBenchmark(); });
	benchmarkThread.join();
}

void Benchmark::Stop()
{
	interruptionFlag.store(true);
}

Benchmark::~Benchmark()
{
	
}

void Benchmark::BeginBenchmark()
{
	std::cout << "Benchmark has started" << std::endl;
	FILE *testFile;
	testFileName = std::string("testFile");
	testFile = fopen(testFileName.c_str(), "w+");
	buffer = CreateBuffer(parameters.BlockSize);
	if (!testFile)
	{
		std::cout << "Cannot create file" << std::endl;
		return;
	}
	for (int iteration = 0; iteration < parameters.RepeatCounter || interruptionFlag.load() == true; iteration++)
	{
		fwrite(buffer, sizeof(buffer), parameters.BlockSize, testFile);
		bytesWritten += parameters.BlockSize;
		if (iteration % 100 == 99)
		{
			NotifyObservers(&bytesWritten);
			bytesWritten = 0;
		}
	}
	DeleteBuffer();
	fclose(testFile);
	remove(testFileName.c_str());
	SetFinished(true);
	bool isFinished = GetFinished();
	NotifyObservers(&isFinished);
	std::cout << "Benchmark has finished" << std::endl;
	std::cout << "Please find your results in " << parameters.logFileName.c_str() << std::endl;
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