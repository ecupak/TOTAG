#pragma once
#include "Vector.h"


template<typename K, typename V>
struct KeyValuePair
{
public:
	// Default constructor needed for Vector class as it makes new objects on creation/resize.
	KeyValuePair() { }

	KeyValuePair(K newKey, V newValue)
		: key{ newKey }
		, value{ newValue }
	{	}

	K key;
	V value;
};


template<typename K, typename V>
class Dictionary
{
public:
	Dictionary() { }


	// Add a new key-value pair.
	void Add(const K& key, const V& value)
	{
		kvps_.Push({ key, value });		
	}


	void Add(const K& key, V* orphanValue)
	{
		kvps_.Push({ key, orphanValue });
	}


	int IndexOf(const K& key) const
	{
		for (int index = 0; index < kvps_.Size(); ++index)
		{
			if (key == kvps_[index].key)
			{
				return index;
			}
		}

		return -1;
	}


	// Returns value of given key.
	V& GetValue(const K& key)
	{
		for (KeyValuePair<K, V> entry in kvps_)
		{
			if (key == entry.key)
			{
				return entry.value;
			}
		}

		// Not sure how to return a default/null/non-success value when the type is unknown.
		throw;
	}


	// Returns value of given index.
	V& GetValueAt(const int index)
	{
		if (index >= 0 && index < kvps_.Size())
		{
			return kvps_[index].value;
		}

		// Not sure how to return a default/null/non-success value when the type is unknown.
		throw;
	}


	// Remove entry with given key.
	void Remove(const K& key)
	{
		int keyIndex = IndexOf(key);

		if (keyIndex != -1)
		{
			kvps_.Remove(keyIndex);
		}
	}


	// Remove entry at given index.
	void RemoveAt(const int index)
	{
		if (index >= 0 && index < kvps_.Size())
		{
			kvps_.Remove(index);
		}
	}


	// Find if a key exists in the dictionary.
	bool Contains(const K& keyQuery) const
	{
		for (KeyValuePair<K, V> entry in kvps_)
		{
			if (keyQuery == entry.key)
			{
				return true;
			}
		}

		return false;
	}


	// Always useful to know.
	int Size() const
	{
		return kvps_.Size();
	}

	
	// Allows iteration with a for-loop (see Iterator.h).
	Iterator<KeyValuePair<K, V>> begin() const { return kvps_.begin(); }
	Iterator<KeyValuePair<K, V>> end() const { return kvps_.end(); }

private:
	// Keys and values of the dictionary.
	Vector<KeyValuePair<K, V>>kvps_;
};

