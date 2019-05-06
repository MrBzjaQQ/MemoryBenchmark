#pragma once
#include <atomic>
class Observer
{
	std::atomic< long long > bytesWritten;
	std::atomic<long long> bytesRead;
	std::atomic<bool> writeStopped;
	std::atomic<bool> readStopped;
protected:
	long long GetBytesWritten();
	long long GetBytesRead();
	void SetBytesWritten(long long bytesCount);
	void SetBytesRead(long long bytesCount);
public:
	Observer();
	~Observer();
	virtual void Update(bool *argument);
	virtual void UpdateWrite(long long *argument);
	virtual void UpdateRead(long long *argument);
	bool GetWriteStopped();
	bool GetReadStopped();
	void SetWriteStopped(bool isStopped);
	void SetReadStopped(bool isStopped);
};

