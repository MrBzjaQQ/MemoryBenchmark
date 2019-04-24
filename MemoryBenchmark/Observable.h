#pragma once
#include "Observer.h"
#include <list>
#include <atomic>
class Observable
{
	std::atomic<bool> isFinished;
	std::atomic<bool> isChanged;
	std::list<Observer*> observers;
protected:
	void SetFinished(bool finished);
	bool GetFinished();
public:
	Observable();
	~Observable();
	void AddObserver(Observer *observer);
	void DeleteObserver(Observer *observer);
	void NotifyObservers(long long *value);
	void NotifyObservers(bool *value);
	void SetChanged();
	bool GetChanged();
};

