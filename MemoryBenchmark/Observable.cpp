#include "Observable.h"



void Observable::SetWriteFinished(bool finished)
{
	isWriteFinished.store(finished);
	for (auto *ob : observers)
		ob->SetWriteStopped(finished);
	
}

void Observable::SetReadFinished(bool finished)
{
	isReadFinished.store(finished);
	for (auto *ob : observers)
		ob->SetReadStopped(finished);
}

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

void Observable::NotifyObserversOnWritten(long long *value)
{
	for (auto *ob : observers)
		ob->UpdateWrite(value);
}

void Observable::NotifyObserversOnRead(long long * value)
{
	for (auto *ob : observers)
		ob->UpdateRead(value);
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
