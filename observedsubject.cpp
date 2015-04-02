#include "observedsubject.h"

ObservedSubject::ObservedSubject()
{

}

ObservedSubject::~ObservedSubject()
{

}

void ObservedSubject::registerObserver(Observer *observer)
{
    observerList.push_back(observer);
}

void ObservedSubject::unregisterObserver(Observer *o)
{
    observerList.remove(o);

}

