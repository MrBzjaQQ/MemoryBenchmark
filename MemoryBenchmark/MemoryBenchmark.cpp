#include "Benchmark.h"
#include <exception>
//TODO добавить флаг -fwrite для работы через fwrite()
//В противном случае вызывать write

//MemoryBenchmark -b 3192 -r 10000000 -f logresults
//-b - блок записи 3192 байта
//-r - число повторений 10 000 000 раз
//-f /home/logresults.csv - запись в файл logresults-read.csv и logresults-write.csv

//MemoryBenchmark -b 3192
//-b - блок записи 3192 байта
//(по умолчанию) -r - число повторений 1 000 000 раз

//MemoryBenchmark -r 10000000
//(по умолчанию)-b - блок записи 128 байт
//-r - число повторений 10 000 000 раз

//MemoryBenchmark
//(по умолчанию) -b - блок записи 128 байт
//(по умолчанию) -r - число повторений 1 000 000 раз
int ParseNumericArgument(std::string);

int main(int argc, char **argv)
{
	BenchmarkParameters arguments;
	for (int i = 1; i < argc; i+=2)
	{
		std::string str(argv[i]);
		if (str.compare("-r") == 0)
		{
			std::string numericValue(argv[i + 1]);
			arguments.RepeatCounter = ParseNumericArgument(numericValue);
		}
		else if (str.compare("-b") == 0)
		{
			std::string numericValue(argv[i + 1]);
			arguments.BlockSize = ParseNumericArgument(numericValue);
		}
		else if (str.compare("-f") == 0) {
			arguments.logFilePrefix = std::string(argv[i + 1]);
		}
		else
		{
			std::cout << "Wrong input parameters" << std::endl;
			return 1;
		}
	}
	BenchmarkResultsLogger logger(arguments.logFilePrefix);
	Benchmark benchmark(arguments);
	benchmark.AddObserver(&logger);
	//logger.Start();
	benchmark.Start();
	logger.Start();
	logger.JoinThread();
}

int ParseNumericArgument(std::string argument)
{
	try
	{
		return std::stoll(argument.c_str());
	}
	catch(std::exception)
	{
		throw new std::exception();
	}
}