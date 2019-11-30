#pragma once
#pragma once
#include <iostream>
#include <assert.h>
#include <iostream>
using namespace std;

template <class T>
class PriorityQueue;
template <class T>
class LinkItorator;

// Node
template <class T>
class LinkNode
{
    template <class T>
    friend class PriorityQueue;
private:
    T m_data;
    LinkNode *m_next;
    LinkNode *m_prev;
    int priority;
};



















// List
template <class T>
class PriorityQueue
{
public:
    // Constructor
    PriorityQueue() : m_size(0), m_root(0), m_lastnode(0)
    {
    }

    // Destructor
    ~PriorityQueue()
    {
        // loop through the PriorityQueue untill m_root is popped off
        while (m_root)
        {
            Pop();
        }
    }


    // PriorityQueue operations
    // Push
    void Push(T newData, int prior)
    {
        // create the new node first and set it up
        LinkNode<T> *node = new LinkNode<T>;
        LinkNode<T> *temp = m_root;

        assert(node != NULL);
        node->m_data = newData;
        node->m_next = NULL;
        node->m_prev = NULL;
        node->priority = prior;


        // if there is at least 1 node in my list
        if (m_root != NULL && m_root->m_next != NULL)
        {
            while (m_root->m_next != NULL && m_root->m_next->priority != (prior + 1))
            {
                m_root = m_root->m_next;
            }



            m_root = temp;
            delete temp;
        }
        else
        {
            if (m_root != NULL && m_root->m_next == NULL)
            {
                if (m_root->priority < node->priority)
                {
                    // with only 1 element
                    m_root->m_next = node;
                    node->m_prev=m_root;
                    m_lastnode = node;
                }
                else
                {
                    // empty list
                    node->m_next = m_root;
                    m_root->m_prev = node;
                    m_lastnode = m_root; 
                }
            }
            else
            {
                m_root = node;
                m_lastnode = node;
            }
        }

        // increment list size
        m_size++;
    }




    // Pop
    void Pop()
    {
        assert(m_lastnode != NULL);

        m_lastnode = NULL;
    
        // Update size
        m_size = (m_size == 0 ? m_size : m_size - 1);
    }


    // Return the value of the first node
    T Front()
    {
        assert(m_root != NULL);

        LinkNode<T> *temp = m_root;

        while (m_root->m_prev != NULL)
        {
            m_root = m_root->m_prev;
        }

        return m_root->m_data;

        m_root = temp;
        delete temp;
    }


    // Getter
    int GetSize()
    {
        return m_size;
    }




    // Print all data in queue
    void GetAllDatas()
    {
        assert(m_root != NULL);

        // temp node
        LinkNode<T> *temp = m_root;

        for (int i = 0; i < m_size; i++)
        {
            cout << m_root->m_data << endl;
            // root equals next
            m_root -> m_next;
        }

        m_root = temp;

        delete temp;
        
    }


private:
    int m_size;              // how big is the list
    LinkNode<T> *m_root;     // the first node
    LinkNode<T> *m_lastnode; // the last node
};
