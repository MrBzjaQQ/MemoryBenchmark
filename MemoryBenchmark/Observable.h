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
	void SetWriteFinished(bool finished);
	void SetReadFinished(bool finished);
	bool GetFinished();
	bool GetWriteFinished();
	bool GetReadFinished();
public:
	Observable();
	~Observable();
	void AddObserver(Observer *observer);
	void DeleteObserver(Observer *observer);
	void NotifyObserversOnWritten(long long *value);
	void NotifyObserversOnRead(long long *value);
	void NotifyObservers(bool *value);
	void SetChanged();
	bool GetChanged();
};

