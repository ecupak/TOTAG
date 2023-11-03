#pragma once

class Observer;

enum class ObjectClass;
enum class Event;


class Subject
{
public:
	Subject();
	~Subject();


	void AddObserver(Observer* observer);


	void Notify(const ObjectClass objectClass, const Event event);


private:
	Observer* (*observers_) { nullptr };
	int observerCurrentIndex_{ 0 };
};

