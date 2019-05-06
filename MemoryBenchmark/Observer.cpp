#include "Observer.h"



long long Observer::GetBytesWritten()
{
	return bytesWritten.load();
}

long long Observer::GetBytesRead()
{
	return bytesRead.load();
}

void Observer::SetBytesWritten(long long bytesCount)
{
	bytesWritten.store(bytesCount);
}

void Observer::SetBytesRead(long long bytesCount)
{
	bytesRead.store(bytesCount);
}

Observer::Observer()
{
	writeStopped.store(false);
	readStopped.store(false);
	bytesWritten.store(0);
	bytesRead.store(0);
}


Observer::~Observer()
{
}

void Observer::Update(bool * argument)
{
	bool isStopped = *argument;
	writeStopped.store(isStopped);
	readStopped.store(isStopped);
}

void Observer::UpdateWrite(long long *argument)
{
	long long *amountOfBytes = static_cast<long long*>(argument);
	bytesWritten += *amountOfBytes;
}

void Observer::UpdateRead(long long * argument)
{
	long long *amountOfBytes = static_cast<long long*>(argument);
	bytesRead += *amountOfBytes;
}

bool Observer::GetWriteStopped()
{
	return writeStopped.load();
}

bool Observer::GetReadStopped()
{
	return readStopped.load();
}

void Observer::SetWriteStopped(bool isStopped)
{
	writeStopped.store(isStopped);
}

void Observer::SetReadStopped(bool isStopped)
{
	readStopped.store(isStopped);
}
