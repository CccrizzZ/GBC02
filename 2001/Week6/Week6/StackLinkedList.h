#pragma once
#include <iostream>
#include "DoubleLinkListh.h"


template <class T>
class StackLinkedList
{
public:
    StackLinkedList::StackLinkedList()
    {

    }

    StackLinkedList::~StackLinkedList()
    {
    }
    
    void Push(T val)
    {
        
        m_container.Push(val);
    }
    
    void Pop()
    {
        m_container.Pop();
    }

    const &T top()
    {
        LinkIterator<T> it;
        it = m_container.Last();
        
        return *it;
    }

    int GetSize()
    {
        return m_container.GetSize();
    }
    
    bool isEmpty()
    {
        return (m_container.GetSize() == 0)
    }
private:
    LinkList<T> m_container;
    int m_size;
};

