#include "precomp.h"
#include "Subject.h"

#include "GameObject.h"
#include "Observer.h"
#include "Event.h"


constexpr int maxObservers_{ 5 };


Subject::Subject()
	: observers_{ new Observer * [maxObservers_] }
{	}


Subject::~Subject()
{
	delete[] observers_;
}


void Subject::AddObserver(Observer* observer)
{
	if (observerCurrentIndex_ < maxObservers_)
	{
		observers_[observerCurrentIndex_++] = observer;
	}
}


void Subject::Notify(const ObjectClass objectClass, const Event event)
{
	for (int index{ 0 }; index < observerCurrentIndex_; ++index)
	{
		observers_[index]->OnNotify(objectClass, event);
	}
}