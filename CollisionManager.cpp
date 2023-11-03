#include "precomp.h"
#include "CollisionManager.h"


CollisionManager::CollisionManager(Vector<GameObject*>& pool)
	: pool_{ pool }
{	}


void CollisionManager::CheckCollisions()
{
	// Initial narrowing possible? only grab things within camera's top and bottom (but extend to level x and x + size for thrown objects?)
	SortAlongXAxis();
	FindXOverlaps();
	NotifyCollisionPairs();
	ResolveAllCollisions();
}


// Sort all objects by their x position in ascending order (bubble sort).
void CollisionManager::SortAlongXAxis()
{	
	int count{ pool_.Size() };

	for (int focusIndex{ 0 }; focusIndex < count - 1; ++focusIndex)
	{
		for (int compareIndex{ focusIndex + 1 }; compareIndex < count; ++compareIndex)
		{
			if (pool_[focusIndex]->shape_.offset_.x > pool_[compareIndex]->shape_.offset_.x)
			{
				// Swap.
				GameObject* temp{ pool_[focusIndex] };

				pool_[focusIndex] = pool_[compareIndex];
				pool_[compareIndex] = temp;
			}
		}
	}
}


// Create potential collision pairs based on x overlap.
void CollisionManager::FindXOverlaps()
{	
	int count{ pool_.Size() };
	xOverlaps_.Reserve(50);

	// If focus's far-right point overlaps any subsequent object's far-left point, they overlap on x-axis.
	for (int focusIndex{ 0 }; focusIndex < count - 1; ++focusIndex)
	{
		int x2{ static_cast<int>(pool_[focusIndex]->shape_.end_.x) };

		for (int compareIndex{ focusIndex + 1 }; compareIndex < count; ++compareIndex)
		{
			// X overlap found. Store as pair to check y overlaps later.
			if (x2 >= static_cast<int>(pool_[compareIndex]->shape_.offset_.x))
			{
				// See if either one is on a layer that is part of the other's mask.
				if (pool_[focusIndex]->collisionLayer_ & pool_[compareIndex]->collisionMask_
					|| pool_[compareIndex]->collisionLayer_ & pool_[focusIndex]->collisionMask_
					)
				{
					xOverlaps_.Push(Pair<GameObject*, GameObject*>{ pool_[focusIndex], pool_[compareIndex] });
				}
			}
			// No more possible overlaps. Start over with next focus.
			else
			{
				break;
			}
		}
	}
}


// Find collisions on the Y axis and notify each other of the other.
void CollisionManager::NotifyCollisionPairs()
{
	for (int index{ 0 }; index < xOverlaps_.Size(); ++index)
	{
		GameObject* first{ xOverlaps_[index].first };
		GameObject* second{ xOverlaps_[index].second };

		// Quicker to check for no collision.
		bool hasNoOverlap{ 
			static_cast<int>(first->shape_.offset_.y) > static_cast<int>(second->shape_.end_.y)
			|| static_cast<int>(first->shape_.end_.y) < static_cast<int>(second->shape_.offset_.y)};

		// Notify colliders of each other.
		if (!hasNoOverlap)
		{
			//first->CollidedWith(second);
			//second->CollidedWith(first);
		}
	}
}


// All collisions have completed for the frame. Participants can now figure out how to respond.
void CollisionManager::ResolveAllCollisions()
{	
	for (int index{ 0 }; index < pool_.Size(); ++index)
	{
		//pool_[index]->ResolveCollisions();
	}
}