#pragma once
#include <atomic>
class Observer
{
	long int bytesWritten;
	std::atomic<bool> stopped;
protected:
	long int GetBytesWritten();
	void SetBytesWritten(long int bytesCount);
public:
	Observer();
	~Observer();
	virtual void update(bool *argument);
	virtual void update(long long *argument);
	bool GetStopped();
	void SetStopped(bool isStopped);
	
};

