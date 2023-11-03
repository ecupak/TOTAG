#pragma once

template<typename T>
class Pooler
{
	Pooler() { }


	void Add(T*)
	{
		// find first inactive element and assign.
		pool_[lastActiveIndex + 1]

	}


	void Remove(T*)
	{
		// find item and set inactive, swap with index of last active.
	}


	void Sort(int direction)
	{
		if (direction == 0)
		{
			return;
		}

		// if positive, ascending order


		// if negative, descending order
	}


	

private:
	template<typename T>
	struct Shell
	{
	public:
		Shell() { }

		void Assign(T* item)
		{
			item_ = item;
		}

		T* operator()
		{
			return (isActive_ ? item_ : nullptr);
		}

		bool isActive_;
		T* item_;
	};

	Shell<T*> pool_[200]{};
	int lastActiveIndex_{ -1 };
};

