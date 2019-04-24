#include "Observer.h"



long int Observer::GetBytesWritten()
{
	return bytesWritten;
}

void Observer::SetBytesWritten(long int bytesCount)
{
	bytesWritten = bytesCount;
}

Observer::Observer()
{
	stopped.store(false);
}


Observer::~Observer()
{
}

void Observer::update(bool * argument)
{
	bool isStopped = *argument;
	stopped.store(isStopped);
}

void Observer::update(long long *argument)
{
	long long *amountOfBytes = static_cast<long long*>(argument);
	bytesWritten += *amountOfBytes;
}

bool Observer::GetStopped()
{
	return stopped.load();
}

void Observer::SetStopped(bool isStopped)
{
	stopped.store(isStopped);
}
