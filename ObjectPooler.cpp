#include "precomp.h"
#include "ObjectPooler.h"

#include "GameObject.h"


constexpr int maxSize_{ 500 };


ObjectPooler::ObjectPooler() 
	: pool_{ new GameObject* [maxSize_] }
{	}



// Add to next "empty" spot in array.
void ObjectPooler::Add(GameObject* go)
{
	if (currentIndex_ < maxSize_)
	{
		pool_[currentIndex_] = go;
		++currentIndex_;
	}
}


// Pooler does not own any object and will not call delete on any removed object.
void ObjectPooler::Remove(GameObject* go)
{
	if (go == nullptr)
	{
		return;
	}

	// Look for the matching object pointer.
	for (int index{ 0 }; index < currentIndex_; ++index)
	{
		if (go == pool_[index])
		{
			// If there is more than 1 active object in the pool
			// and the current object is not the last active object.
			if (currentIndex_ > 1 && index < currentIndex_ - 1)
			{
				// Replace current object with last active object.
				pool_[index] = pool_[currentIndex_ - 1];
			}

			// "Remove" last active object.
			pool_[currentIndex_ - 1] = nullptr;
			--currentIndex_;
		}
	}
}


const int ObjectPooler::GetSize() const
{
	return currentIndex_;
}


GameObject* const ObjectPooler::operator[](const int index) const
{
	return pool_[index];
}