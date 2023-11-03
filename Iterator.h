// [Credit] https://stackoverflow.com/questions/8164567/how-to-make-my-custom-type-to-work-with-range-based-for-loops
// Gives ability to be looped over range-based for loops.


#pragma once
template <typename T>
class Iterator
{
    /*
        IMPORTANT
        Any class using this header must also implement the following 2 methods (just cut and paste them in).        
            <<val>> must be a pointer to the container.
            <<len>> must be the size of the container.
            <<typename>> must be the same type as the container's elements. If the container is a template, just use the same typename alias.
        
        Iterator<<typename>> begin() const { return Iterator(<<val>>); }
        Iterator<<typename>> end() const { return Iterator(<<val>> + <<len>>); }


        ALSO
        Any class wrapping a container must implement these, but can call the same method on the container as the argument
            <<typename>> must be the same type as the container's elements.

        Iterator<<typename>> begin() const { return myContainer.begin(); }
        Iterator<<typename>> end() const { return myContainer.end(); }
    */

public:
    // Gain reference to the container.
    Iterator(T* ptr) 
        : ptr_{ ptr }
    {   }


    // Advance to next element. All chaining.
    Iterator operator++() 
    { 
        ++ptr_;
        return *this; 
    }
    
    
    // Comparison. Used to make sure not at end of container (by checking against end iterator which is pointer after last element).
    bool operator!=(const Iterator<T>& other) const 
    { 
        return ptr_ != other.ptr_;
    }


    // Dereferences current element to provide in the for-loop.
    const T& operator*() const 
    { 
        return *ptr_; 
    }


private:
    // Our container being looped over.
    T* ptr_;
};