#ifndef OBSERVEDSUBJECT_H
#define OBSERVEDSUBJECT_H

#include <list>
#include "observer.h"

class ObservedSubject
{
public:
    ObservedSubject();
    ~ObservedSubject();

    void registerObserver(Observer *observer);
    void unregisterObserver(Observer *observer);
    virtual void notifyObservers() = 0;

protected:
    std::list<Observer*> observerList;
};

#endif // OBSERVEDSUBJECT_H
