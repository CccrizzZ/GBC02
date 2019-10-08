#pragma once
#include <iostream>
#include <assert.h>
using namespace std;

template<class T>
class Stack
{
public:
    Stack();
    Stack(int size, int growBy = 1) : m_size(0), m_top(-1), m_array(0), m_growSize(0)
    {
        if (size)
        {
            m_size = size;
            m_array = new T[m_size];
            assert(m_array != NULL);
            memset(m_array, 0, sizeof(T) * m_size);

            m_growSize = (growBy > 0 ? growBy : 0);
       
        }       
    }

    ~Stack(){
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

        // If this stack is full then expand it
        if (isFull())
        {
            Expand();
        }

        // Increment m_pop then insert val into the stack
        m_array[++m_top] = val;
        
    }


    // Pop
    void Pop()
    {
        if (!isEmpty())
        {
            m_top--;
        }
    }

    // top
    const T& top()
    {
        assert(m_array != NULL);
        assert(m_top >= 0);
        
        return m_array[m_top];
    }


    int GetSize()
    {
        return m_top + 1;
    }


    int GetMaxSize()
    {
        return m_size;
    }


    bool isEmpty()
    {
        return (m_top == -1);
    }

    bool isFull()
    {
        return (m_top);
    }

private:
    bool Expand()
    {
        if (m_growSize <= 0)
        {
            return false;
        }
        
        assert(m_array != NULL);

        // Create a larger temp array
        T* temp = new T[m_size + m_growSize];
        assert(temp != NULL);

        // Copy elements from the original array to the new array
        memcpy(temp, m_array, sizeof(T) * m_size);

        delete[] m_array;


    }

private:
    T* m_array;
    int m_top;
    int m_size;
    int m_growSize;
};