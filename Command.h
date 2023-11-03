#pragma once

class Actor;


class Command
{
public:
	Command(const Actor* actor = nullptr)
		: actor_{ actor }
		, direction_{ 0, 0 }
	{	}

	Command(const Actor* actor, const int xDirection, const int yDirection) 
		: actor_{ actor }
		, direction_{ xDirection, yDirection }
	{	}

	Command(const int xDirection, const int yDirection)
		: direction_{ xDirection, yDirection }
	{	}

	virtual ~Command() { }
	virtual void Execute(const float deltaTime) = 0; // pure virutal function - must be definied in derived class. this class cannot be instantiated.

protected:
	const Actor* actor_{ nullptr };
	const int2 direction_{ 0, 0 };
};

