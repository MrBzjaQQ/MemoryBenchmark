#include "Observer.h"



Observer::Observer()
{

}


Observer::~Observer()
{
}

void Observer::Update(bool * argument)
{
}

void Observer::UpdateWrite(LogRecord record)
{
	writeRecords.push_back(record);
}

void Observer::UpdateRead(LogRecord record)
{
	readRecords.push_back(record);
}
