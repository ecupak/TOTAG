#include "precomp.h"
#include "ObjectCollisionManager.h"

#include "Shape.h"
#include "GameObject.h"
#include "Camera.h"
#include "Hitbox.h"
#include "PixelChecker.h"


ObjectCollisionManager::ObjectCollisionManager(const Camera& camera, GameObject* const activeList[])
	: camera_{ camera }
	, activeList_{ activeList }
	, pixelChecker_{ new PixelChecker{} }
{	}


ObjectCollisionManager::~ObjectCollisionManager()
{
	delete pixelChecker_;
	delete[] onstageObjects_;
	delete[] objectXPairs_;
	delete[] objectOverlaps_;
	delete[] pixelPairs_;
}


void ObjectCollisionManager::ProcessCollisions(const int activeCount)
{
	GatherOnstageObjects(activeCount);

	SortAlongXAxis();

	FindXOverlaps();

	FindYOverlaps();

	FindPixelOverlaps();

	NotifyCollisionPairs();
}


// Objects within the extended camera bounds are "onstage" and should do their Update() methods.
void ObjectCollisionManager::GatherOnstageObjects(const int activeCount)
{
	// "Resize" previous array to hold at least all of the items in the pooler.
	onstageCurrentIndex_ = 0;
	delete[] onstageObjects_;
	onstageObjects_ = new GameObject* [activeCount];

	// Mark 'onstage' if object center is within the extended camera bounds. Remove 'onstage' when they leave camera bounds.
	// This applies to the player as well - some situations (cutscenes or menus) they may not be onstage.
	// Onstage objects will be drawn and run their Update() method.	
	int objectTypesOfInterest{ ObjectType::Actor | ObjectType::Prop };
	for (int poolerIndex{ 0 }; poolerIndex < activeCount; ++poolerIndex)
	{
		GameObject& go{ *activeList_[poolerIndex] };
		
		if (go.objectType_ & objectTypesOfInterest)
		{
			if (go.shape_->center_.x > camera_.shape_->start_.x
				&& go.shape_->center_.x < camera_.shape_->end_.x
				&& go.shape_->center_.y > camera_.shape_->start_.y
				&& go.shape_->center_.y < camera_.shape_->end_.y)
			{
				go.isOnstage_ = true;
				onstageObjects_[onstageCurrentIndex_] = &go;
				++onstageCurrentIndex_;
			}
			else
			{
				go.isOnstage_ = false;
			}
		}
	}
}


// Sort all objects by their x position in ascending order (bubble sort).
void ObjectCollisionManager::SortAlongXAxis()
{	
	for (int focusIndex{ 0 }; focusIndex < onstageCurrentIndex_ - 1; ++focusIndex)
	{
		for (int compareIndex{ focusIndex + 1 }; compareIndex < onstageCurrentIndex_; ++compareIndex)
		{
			if (onstageObjects_[focusIndex]->shape_->start_.x > onstageObjects_[compareIndex]->shape_->start_.x)
			{
				// Swap.
				GameObject* temp{ onstageObjects_[focusIndex] };

				onstageObjects_[focusIndex] = onstageObjects_[compareIndex];
				onstageObjects_[compareIndex] = temp;
			}
		}
	}
}


// Create potential collision pairs based on x overlap.
void ObjectCollisionManager::FindXOverlaps()
{	
	xPairsCurrentIndex_ = 0;

	// If not even a pair exist, abort.
	if (onstageCurrentIndex_ < 2)
	{
		return;
	}

	// Prepare new array.
	delete[] objectXPairs_;
	objectXPairs_ = new GameObject * [onstageCurrentIndex_ * 10];

	// If focus's far-right point overlaps any subsequent object's far-left point, they overlap on x-axis.
	for (int focusIndex{ 0 }; focusIndex < onstageCurrentIndex_ - 1; ++focusIndex)
	{
		int rightPosition{ static_cast<int>(onstageObjects_[focusIndex]->shape_->end_.x) };

		for (int compareIndex{ focusIndex + 1 }; compareIndex < onstageCurrentIndex_; ++compareIndex)
		{
			// X overlap found. Store as pair to check y overlaps later.
			if (rightPosition >= static_cast<int>(onstageObjects_[compareIndex]->shape_->start_.x))
			{
				// See if either one is on a layer that is part of the other's mask.
				if (onstageObjects_[focusIndex]->objectLayer_ & onstageObjects_[compareIndex]->objectMask_
					|| onstageObjects_[compareIndex]->objectLayer_ & onstageObjects_[focusIndex]->objectMask_
					)
				{
					// Store as sequential items in array.
					objectXPairs_[xPairsCurrentIndex_] = onstageObjects_[focusIndex];
					objectXPairs_[xPairsCurrentIndex_ + 1] = onstageObjects_[compareIndex];
					xPairsCurrentIndex_ += 2;
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


// Find collisions on the Y axis.
void ObjectCollisionManager::FindYOverlaps()
{
	overlapsCurrentIndex_ = 0;
	
	// If not even a pair overlap, abort.
	if (xPairsCurrentIndex_ < 2)
	{
		return;
	}

	// Prepare new array.
	delete[] objectOverlaps_;
	objectOverlaps_ = new GameObject * [xPairsCurrentIndex_];

	for (int index{ 0 }; index < xPairsCurrentIndex_; index += 2)
	{
		GameObject& first{ *objectXPairs_[index] };
		GameObject& second{ *objectXPairs_[index + 1] };

		// Quicker to check for no collision.
		bool hasNoOverlap{ 
			static_cast<int>(first.shape_->start_.y) > static_cast<int>(second.shape_->end_.y)
			|| static_cast<int>(first.shape_->end_.y) < static_cast<int>(second.shape_->start_.y)};

		// Collect overlaps into new set.
		if (!hasNoOverlap)
		{
			objectOverlaps_[overlapsCurrentIndex_] = &first;
			objectOverlaps_[overlapsCurrentIndex_ + 1] = &second;
			overlapsCurrentIndex_ += 2;
		}
	}
}


// Find which hitboxes overlap, and check for pixel collisions with them.
// Calculate the overlap box here instead of in pixel checker, because the fh start value can be reused for each inner loop cycle.
void ObjectCollisionManager::FindPixelOverlaps()
{
	pixelCurrentIndex_ = 0;

	// If not even a pair overlap, abort.
	if (overlapsCurrentIndex_ < 2)
	{
		return;
	}

	// Prepare new array.
	delete[] pixelPairs_;
	pixelPairs_ = new GameObject* [overlapsCurrentIndex_];

	// Process overlaps in pairs.
	for (int index{ 0 }; index < overlapsCurrentIndex_; index += 2)
	{
		bool pixelOverlapFound{ false };

		GameObject& first{ *objectOverlaps_[index] };
		GameObject& second{ *objectOverlaps_[index + 1] };

		// Loop through the first go's hitboxes.
		for (int fi{ 0 }; fi < first.hitboxCount_; ++fi)
		{
			Hitbox fh{ first.hitboxes_[fi] }; 

			if (!(fh.mode_ & Hitbox::Mode::Object))
			{
				continue;
			}

			fh.offset_.x += static_cast<int>(first.shape_->start_.x);
			fh.offset_.y += static_cast<int>(first.shape_->start_.y);

			// Compare them to the second go's hitboxes.
			for (int si{ 0 }; si < second.hitboxCount_; ++si)
			{
				Hitbox sh{ second.hitboxes_[si] };

				if (!(sh.mode_ & Hitbox::Mode::Object))
				{
					continue;
				}

				sh.offset_.x += static_cast<int>(second.shape_->start_.x);
				sh.offset_.y += static_cast<int>(second.shape_->start_.y);

				// Calculate overlap.
				int2 overlapStart{
					max(fh.offset_.x, sh.offset_.x),
					max(fh.offset_.y, sh.offset_.y)
				};

				int2 overlapEnd{
					min(fh.offset_.x + fh.size_.x, sh.offset_.x + sh.size_.x),
					min(fh.offset_.y + fh.size_.y, sh.offset_.y + sh.size_.y)
				};

				// Confirm overlap.
				if (overlapEnd.x > overlapStart.x && overlapEnd.y > overlapStart.y)
				{
					// If the object has no tangible sprite, have it make one of its current image.
					first.PrepareSprite();
					second.PrepareSprite();

					if (fh.mode_ & Hitbox::Mode::Trigger
						|| sh.mode_ & Hitbox::Mode::Trigger
						|| pixelChecker_->DoObjectsTouch(first, second, overlapStart, overlapEnd))
					{
						pixelPairs_[pixelCurrentIndex_] = &first;
						pixelPairs_[pixelCurrentIndex_ + 1] = &second;
						pixelCurrentIndex_ += 2;

						pixelOverlapFound = true;

						break;
					}
				}
			}

			if (pixelOverlapFound)
			{
				break;
			}
		}
	}
}



void ObjectCollisionManager::NotifyCollisionPairs()
{
	// First notify all objects of what they hit or got hit by.
	for (int index{ 0 }; index < pixelCurrentIndex_; index += 2)
	{
		GameObject& first{ *pixelPairs_[index] };
		GameObject& second{ *pixelPairs_[index + 1] };

		first.OnObjectCollision(&second);
		second.OnObjectCollision(&first);
	}

	// Then let them decide what to do about it.
	for (int index{ 0 }; index < pixelCurrentIndex_; ++index)
	{
		pixelPairs_[index]->ResolveObjectCollisions();
	}
}