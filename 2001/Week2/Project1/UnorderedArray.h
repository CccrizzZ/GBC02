#pragma once
#include <iostream>
#include <cassert>
#include <string.h>
using namespace std;

template <class T>
class UnorderedArray
{
public:
  // Constructor
  UnorderedArray(int size, int growBy = 1) :
  m_array(NULL),
  m_maxSize(0),
  m_growSize(0),
  m_numElements(0)
  {
    if (size)
    {
      m_maxSize = size;
	  m_array = new T[m_maxSize];
	  memset(m_array, 0, sizeof(T) * m_maxSize);
	  m_growSize = ((growBy > 0) ? growBy : 0);
    }
  }

  // Destructor
  ~UnorderedArray()
  {
    if (m_array != NULL)
    {
		delete[] m_array;
		m_array = NULL;
    }
  }


  // Push
  void Push(T val)
  {
    assert(m_array != NULL);
    if (m_numElements >= m_maxSize)
    {
		Expand();
    }
    m_array[m_numElements] = val;
    m_numElements++;
  }


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


  // Overload the [ ] operator
  T& operator[](int index)
  {
    assert(m_array != NULL && index < m_numElements); // Debug only
    return m_array[index];
  }


  // Searching
  // Linear search
  int LinearSearch(T val)
  {
    assert(m_array != NULL); // Debug only

    // Big-O of O(N)
    for (int i = 0; i < m_numElements; i++)
    {
      if (m_array[i] == val)
      {
        return i;
      }
    }
    // return -1 if nothing found
    return -1;
  }


  // Remove all items from the array
  void clear()
  {
    m_numElements = 0;
  }


  // Get functions
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


  // Set functions
  void setGrowSize(int val)
  {
    assert(val <= 0); // Debug
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
