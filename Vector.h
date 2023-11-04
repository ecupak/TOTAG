#pragma once
#include "Iterator.h"


// [Credit] https://www.geeksforgeeks.org/how-to-implement-our-own-vector-class-in-c/
// Entire class heavily inspired by the above.
template<typename T>
class Vector
{
public:
	// Number of elements that have been added. Can never be more than capacity (will cause a resize event).
	int current_;
	
	
	// Constructor creates a vector of size 1.
	Vector() 
	{
		vector_ = new T[1];
		capacity_ = 1;
		current_ = 0;
	}


	// Get rid of dynamic memory.
	~Vector()
	{
		delete[] vector_;
	}


	// Clears the vector and sets size.
	void Reserve(int requiredCapacity)
	{
		delete[] vector_;
		vector_ = new T[requiredCapacity];

		capacity_ = requiredCapacity;
		current_ = 0;
	}


	// "Clears" the vector. Actually just sets current index back to 0. Keeps capacity.
	void Clear()
	{
		current_ = 0;
	}


	// Add a copy of the element to the vector.
	void Push(const T element)
	{
		// Copy old vector into new one with double capacity if current capacity reached.
		if (current_ == capacity_)
		{			
			T* temp = new T[2 * capacity_];

			for (int index = 0; index < capacity_; ++index)
			{
				temp[index] = vector_[index];
			}

			delete[] vector_;
			vector_ = temp;

			capacity_ *= 2;
		}

		// Add element to the end.
		vector_[current_] = element;		
		++current_;
	}


	// [Credit] https://www.learncpp.com/cpp-tutorial/overloading-the-subscript-operator/
	// Return vector values.
	T& operator[](const int index) const
	{
		return vector_[index];
	}


	// Remove last element from vector.
	void Pop()
	{
		if (index >= 0 && index <= current_)
		{
			--current_;			
		}
	}


	// Remove an element and shift remaining elements down to
	void Remove(const int index)
	{
		if (index >= 0 && index <= current_)
		{
			// Make last element inaccessible and reflect size has decrased by 1.
			--current_;


			// Bump all elements down 1. Would like to have another way to store the index of each gap, but that requires a *gasp* vector.
			for (int elementIndex = index; elementIndex < current_; ++elementIndex)
			{
				vector_[elementIndex] = vector_[elementIndex + 1];
			}
		}
	}


	// Always useful to know.
	int Size() const
	{
		return current_;
	}


	// Less useful.
	int GetCapacity() const
	{
		return capacity_;
	}

	
	// Allows iteration with a for-loop (see Iterator.h).
	Iterator<T> begin() const { return Iterator(vector_); }
	Iterator<T> end() const { return Iterator(vector_ + current_ ); }

	private:
		// A pointer of our type. Will use pointer arithmitic to get elements past 0. Will reassign as capacity is reached.
		T* vector_;


		// Amount that our vector has allocated memory for with new.
		int capacity_;
};