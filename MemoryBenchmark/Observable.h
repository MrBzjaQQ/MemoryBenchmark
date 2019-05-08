#pragma once
#include "Observer.h"
#include <list>
#include <atomic>
class Observable
{
	std::atomic<bool> isWriteFinished;
	std::atomic<bool> isReadFinished;
	std::atomic<bool> isChanged;
	std::list<Observer*> observers;
protected:
	bool GetFinished();
	bool GetWriteFinished();
	bool GetReadFinished();
public:
	Observable();
	~Observable();
	void AddObserver(Observer *observer);
	void DeleteObserver(Observer *observer);
	void NotifyObserversOnWritten(LogRecord record);
	void NotifyObserversOnRead(LogRecord record);
	void NotifyObservers(bool *value);
	void SetChanged();
	bool GetChanged();
};

