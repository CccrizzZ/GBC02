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


  // Sorting
  // Bubble Sort O(N^2)
  void BubbleSort()
  {
    assert(m_array!=NULL);

    T temp;
    
  
    for (int i = 0; i < m_numElements-1; i++)
    {
      for (int j = 0; j < m_numElements-i-1; j++)
      {
        if (m_array[j] > m_array[j + 1])
        {
          temp = m_array[j];
          m_array[j] = m_array[j+1];
          m_array[j+1] = temp;
        }
      }
    }
  }


  // Selection Sort O(N^2)
  void SelectionSort()
  {
    assert(m_array!=NULL);

    T temp;
    int min = 0;

    // Keeps track of the left-most(lowest) element being swapped with
    for (int k = 0; k < m_numElements - 1; k++)
    {
      min = k;
      // Used to traverse array and find the lowest value in the set
      for (int i = k+1; i < m_numElements; i++)
      {
        if (m_array[i] < m_array[min])
        {
          min = i;
        }
      }

      if (m_array[k] > m_array[min])
      {
        temp = m_array[k];
        m_array[k] = m_array[min];
        m_array[min] = temp;
      }
      
    }
  }


  // Insertion Sort O(N^2)
  void InsertionSort()
  {
    assert(m_array!=NULL);
    T temp;
    int i = 0;

    for (int k = 1; k < m_numElements; k++)
    {
      temp = m_array[k];
      i = k;
      while (i > 0 && m_array[i-1] >= temp)
      {
        m_array[i] = m_array[i-1];
        i--;
      }
      m_array[i] = temp;
    }
    
  }

  // Merge sort
  void MergeSort(){
    assert(m_array!=NULL);

    // temp array mimic the m_array
    T* tempArray = new T[m_numElements];
    assert(tempArray!=NULL);

    // first merge sort
    MergeSort(tempArray, 0, m_numElements-1);
    
    // delete temp array
    delete[] tempArray;
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

  // Merge sort recursion
  void MergeSort(T* tempArray, int lowerBound, int upperBound){
    // Base case
    if (lowerBound == upperBound)
    {
      return;
    }
    int mid = (lowerBound + upperBound) >> 1; // devide by 2
    
    MergeSort(tempArray, lowerBound, mid);
    MergeSort(tempArray, mid + 1, upperBound);
    Merge(tempArray, lowerBound, mid + 1, upperBound);
  }

  void Merge(T* tempArray, int low, int mid, int upper){
    int tempLow = low;
    int tempMid = mid-1;
    int index = 0;

    // Swap
    while (low <= tempMid && mid <= upper)
    {

      if (m_array[low] < m_array[mid])
      {
        tempArray[index++] = m_array[low++];
      }
      else
      {
        tempArray[index++] = m_array[mid++];
      }


      while (low <= tempMid)
      {
        tempArray[index++] = m_array[mid++];
      }


      for (int i = 0; i < upper - tempLow + 1; i++)
      {
        m_array[tempLow + 1] = tempArray[i];
      }
    }
  }



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
