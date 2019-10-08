#pragma once
#include <iostream>
#include <assert.h>
#include "UnorderedArray.h"
using namespace std;



template<class T>
class Stack
{
public:
    Stack(int size, int growBy = 1)
    {
        assert(size > 0 && growBy >= 0);
        m_container = new UnorderedArray<T>(size, growBy);
        assert(m_container!=NULL);
    }
    
    ~Stack()
    {
        if (m_container != NULL)
        {
            delete m_container;
            m_container = NULL;
        }
        
    }


    void Push(T val)
    {
        m_container->Push(val);
    }
    

    void Pop()
    {
        m_container->Pop();
    }


    void top()
    {
        return (*m_container)[m_container->GetSize() - 1];
    
    }


    int GetSize()
    {
        return m_container->getSize();
    }


    int GetMaxSize()
    {
        return m_container->getMaxSize();
    }


    bool isEmpty()
    {
        assert(m_container != NULL)
        return (m_container->GetSize() == 0);
    }

private:
    // Variables
    UnorderedArray<T>* m_container;

};