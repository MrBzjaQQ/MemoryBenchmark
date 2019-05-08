#include "Benchmark.h"
#include <exception>
//TODO �������� ���� -fwrite ��� ������ ����� fwrite()
//� ��������� ������ �������� write

//MemoryBenchmark -b 3192 -r 10000000 -f logresults
//-b - ���� ������ 3192 �����
//-r - ����� ���������� 10 000 000 ���
//-f /home/logresults.csv - ������ � ���� logresults-read.csv � logresults-write.csv

//MemoryBenchmark -b 3192
//-b - ���� ������ 3192 �����
//(�� ���������) -r - ����� ���������� 1 000 000 ���

//MemoryBenchmark -r 10000000
//(�� ���������)-b - ���� ������ 128 ����
//-r - ����� ���������� 10 000 000 ���

//MemoryBenchmark
//(�� ���������) -b - ���� ������ 128 ����
//(�� ���������) -r - ����� ���������� 1 000 000 ���
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