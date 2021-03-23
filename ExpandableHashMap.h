// ExpandableHashMap.h

#include <vector>
#include <list>

#include "provided.h"

using namespace std;

// You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);

	~ExpandableHashMap();

	void reset();			// resets the hashmap back to 8 buckets, deletes all items

	int size() const;		// return the number of associations in the hashmap

	void associate(const KeyType& key, const ValueType& value);

    // If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value, and if the hashmap is allowed to be modified, to
	// modify that value directly within the map (the second overload enables
	// this). Using a little C++ magic, we have implemented it in terms of the
	// first overload, which you must implement.

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}
	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:

	struct Pair
	{
		Pair(KeyType k, ValueType v)
		{
			m_key = k;
			m_value = v;
		}
		KeyType m_key;
		ValueType m_value;
	};

	vector< list<Pair> > m_table;              //each element of the vector is a bucket,
												//each bucket corresponds to a list
												//each element in the list points to a Pair
												//each Pair is key, value
												//whatever they may be

	int m_currentMaxSize;		//current capacity of the table

	double m_maxLoad;          //max load as specified during construction

	double currentLoad()      //returns how full the table currently is
	{
		return (size()*1.0 / m_currentMaxSize);
	}

	void cleanUp()      //clears all the elements
	{
		for (int i = 0; i < m_table.size(); i++)
		{
			m_table[i].clear();
		}
		m_table.clear();
	}

};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	m_currentMaxSize = 8;
	m_maxLoad = maximumLoadFactor;

	list<Pair> emptyVal;
	m_table.resize(m_currentMaxSize, emptyVal);
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	cleanUp();
	//no elements left
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	cleanUp();
	//no elements left

	//reset size to 8
	m_currentMaxSize = 8;

	//fill again with empty lists
	list<Pair> emptyVal;
	m_table.resize(m_currentMaxSize, emptyVal);
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	int co = 0;
	for (int i = 0; i < m_table.size(); i++)           //for each bucket
		for (typename list<Pair>::const_iterator it = m_table[i].begin(); it != m_table[i].end(); ++it)      //go through its list
			co++;

	return co;
	
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	//figuring out the bucket
	unsigned int hasher(const KeyType & k); // prototype
	unsigned int h = hasher(key);
	unsigned int nBucket = h % m_currentMaxSize;


	for (typename list<Pair>::iterator it = m_table[nBucket].begin(); it != m_table[nBucket].end(); ++it)
	{
		if (it->m_key == key)
		{
			it->m_value = value;
			return;
		}
	}			

	m_table[nBucket].push_back(Pair(key, value));
	//cerr << "Adding a new element " <<endl;
	//cerr << "Size = " << size() << endl;
	//cerr << "Current Load = " << currentLoad() << endl;


	if (currentLoad() > m_maxLoad)
	{
		//cerr << "Load > " << m_maxLoad << " rehashing" << endl;
		//create a temp table
		vector< list<Pair> > m_tableTemp;         
		list<Pair> emptyVal;
		m_tableTemp.resize(m_currentMaxSize*2, emptyVal);


		for (int i = 0; i < m_table.size(); i++)		//for each bucket in the original table
		{													
			for (typename list<Pair>::iterator it = m_table[i].begin(); it != m_table[i].end(); ++it) //for each element in the buckets
			{
				//figuring out the new bucket
				unsigned int hasher(const KeyType & k); // prototype
				unsigned int h = hasher(it->m_key);
				unsigned int newBucket = h % (m_currentMaxSize*2);

				//add that bucket to the hash table
				m_tableTemp[newBucket].push_back(Pair(it->m_key, it->m_value));

			}
		}
		//the temp table now has all the associations


		cleanUp();      //clean up the old table

		m_currentMaxSize = m_currentMaxSize * 2;		//resize it
		m_table.resize(m_currentMaxSize, emptyVal);        
		


		for (int i = 0; i < m_table.size(); i++)			//transfer all the elements of the temp table to the current table
		{
			m_table[i].splice(m_table[i].begin(), m_tableTemp[i]);      //copy over list by list   

			m_tableTemp[i].clear();                 //then clear that list
		}
		m_tableTemp.clear();     //then clear the temp table

		//cerr << "New Load = " << currentLoad() << endl;

	}

}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int hasher(const KeyType & k); // prototype
	unsigned int h = hasher(key);

	unsigned int nBucket = h % m_currentMaxSize;

	for (typename list<Pair>::const_iterator it = m_table[nBucket].begin(); it != m_table[nBucket].end(); ++it)
	{
		if (it->m_key == key)
			return &(it->m_value);
	}

	return nullptr;
}

