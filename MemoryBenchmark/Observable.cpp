#include "Observable.h"



void Observable::SetFinished(bool finished)
{
	isFinished.store(finished);
}

bool Observable::GetFinished()
{
	return isFinished.load();
}

Observable::Observable()
{
	isChanged.store(false);
	isFinished.store(false);
}


Observable::~Observable()
{
}

void Observable::AddObserver(Observer * observer)
{
	observers.push_back(observer);
}

void Observable::DeleteObserver(Observer * observer)
{
	observers.remove(observer);
}

void Observable::NotifyObservers(long long *value)
{
	for (auto *ob : observers)
		ob->update(value);
}

void Observable::NotifyObservers(bool * value)
{
	for (auto *ob : observers)
		ob->update(value);
}

void Observable::SetChanged()
{
	isChanged.store(true);
}

bool Observable::GetChanged()
{
	return isChanged.load();
}
