#pragma once
#include "LinkedList.h"
#include <iostream>
using namespace std;

// List
template <class T>
class PriorityQueue
{
public:

    PriorityQueue(int size)
    {
        assert(size > 0);
        m_size = size;
    }

    ~PriorityQueue() 
    {
    }
    
    // Push according to the priority
    void push(T val, int prio)
    {
        if (m_elements.GetSize() < m_size)
        {
            m_elements.Push(val, prio);
        }
    }
    
    // Pop the first element
    void pop()
    {
        m_elements.Pop();
    }

    // Get the first element
    T &front()
    {
        LinkIterator<T> it;
        it = m_elements.Begin(); 

        return *it; 
    }

    // Get the last element
    T &back()
    {
        LinkIterator<T> it;
        it = m_elements.Last(); 

        return *it;
    }
    // Helper Functions
    int GetSize()
    {
        return m_elements.GetSize();
    }

    bool isEmpty()
    {
        return (m_elements.GetSize() == 0);
    }
    void Resize(int size)
    {
        assert(size > 0);
        m_size = size;
    }

private:
    LinkList<T> m_elements; // Link List
    int m_size;             // Size 
};
