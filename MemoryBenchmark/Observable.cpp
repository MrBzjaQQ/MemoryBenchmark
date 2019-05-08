#include "Observable.h"


bool Observable::GetFinished()
{
	return isWriteFinished.load() & isReadFinished.load();
}

bool Observable::GetWriteFinished()
{
	return isWriteFinished.load();
}

bool Observable::GetReadFinished()
{
	return isReadFinished.load();
}

Observable::Observable()
{
	isChanged.store(false);
	isWriteFinished.store(false);
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

void Observable::NotifyObserversOnWritten(LogRecord record)
{
	for (auto *ob : observers)
		ob->UpdateWrite(record);
}

void Observable::NotifyObserversOnRead(LogRecord record)
{
	for (auto *ob : observers)
		ob->UpdateRead(record);
}

void Observable::NotifyObservers(bool * value)
{
	for (auto *ob : observers)
		ob->Update(value);
}

void Observable::SetChanged()
{
	isChanged.store(true);
}

bool Observable::GetChanged()
{
	return isChanged.load();
}
