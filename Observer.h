#pragma once

enum class ObjectClass;
enum class Event;


class Observer
{
public:
	virtual ~Observer() {}
	virtual void OnNotify(const ObjectClass objectClass, const Event event) = 0;
};