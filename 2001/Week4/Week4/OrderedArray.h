#pragma once
#include <cassert>
using namespace std;

template <class T>
class OrderedArray
{
public:

	// Constructor
	OrderedArray(int size, int growBy = 1) :
	m_array(NULL), m_maxSize(0), m_growSize(0), m_numElements(0)
	{
		if (size)
		{
			m_array = new T[m_maxSize];
			memset(m_array, 0, sizeof(T) * m_maxSize);
			m_growSize = ((growBy > 0) ? growBy : 0);

		}
	}


	// Destructor
	~OrderedArray()
	{
		if (m_array != NULL)
		{
			delete[] m_array;
			m_array = NULL;
		}
	}


	// Insertion
	int Push(T val)
	{
		assert(m_array != NULL);

		if (m_numElements >= m_maxSize)
		{
			// Expand();
		}

		// Create a gap in the array and insert new element
		int i, k;
		for (i = 0; i < m_numElements; i++)
		{
			if (m_array[i] > val)
			{
				break;
			}
		}
		for (k = m_numElements; k > i; k--) {
			m_array[k] = m_array[k-1];

		}

		// insert the new element
		m_array[i] = val;
		m_numElements++;

		// returning the index
		return i;
	}


	// Deletion (2 methods)

	// Pop
	// Remove last item that was iterated
  void Pop()
  {
    // Gain a little performance boost
    if (m_numElements > 0)
    {
      m_numElements--;
    }
  }

	// Remove
  // Remove an index from the array by overriding the index to delete
  void Remove(int index)
  {
	  assert(m_array != NULL);
    if (index >= m_numElements)
    {
      return;
    }

    // Expensive to execute (O(N))
    for (int i = index; i < m_numElements; i++)
    {
      if (i + 1 < m_numElements)
      {
        m_array[i] = m_array[i+1];
      }
    }
    m_numElements--;
  }



	// Overloaded [] operator
	const T& operator[](int index)
	{
		assert(m_array != NULL && index < m_numElements);
		return m_array[index];
	}


	// Search
	// Binary Search
	int BinarySearch(T searchKey)
	{
		assert(m_array != NULL);

		int lowerbound = 0;
		int upperbound = m_numElements - 1;
		int current = 0;

		while (1)
		{
			current = (lowerbound + upperbound) >> 1; // Bit shift (aka devide by 2, but more efficient)

			if (m_array[current] == searchKey) // middle element of the array equals to search key
			{
				return current; // FOUND
			}
			else if (lowerbound > upperbound) // checked through my entire array
			{
				return -1; // NOT FOUND
			}
			else
			{
				if (m_array[current] < searchKey)
				{
					// shift lowerbound up to half-way
					lowerbound = current + 1;
				}
				else
				{
					// first half of the array
					upperbound = current - 1;
				}
			}
		}
		return -1;
	}



	// Clear
	void Clear() 
	{
		m_numElements = 0;
	}

	// Getter
	int getSize() 
	{
		return m_numElements;
	}

	int getMaxSize() 
	{
		return m_maxSize;
	}

	int getGrowSize() 
	{
		return m_growSize;
	}

	// Setter
	void setGrowSize(int val) 
	{
		assert(val >= 0);
		m_growSize = val;
	}

private:
	// Expand
  bool Expand()
  {
    if (m_growSize <= 0)
    {
      return false;
    }

    T* temp = new T[m_maxSize + m_growSize];
    assert(temp != NULL); // Debug

    // memory copy
    memcpy(temp, m_array, sizeof(T) * m_maxSize);

    delete[] m_array;
    m_array = temp;
    m_maxSize += m_growSize;

    return true;
  }
private:
	T* m_array;
	int m_maxSize;      // max size of array
	int m_growSize;     // how much can this array grow
	int m_numElements;  // how many items are currently in the array


};
