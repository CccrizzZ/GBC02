#pragma once
#include<iostream>
#include<assert.h>

template<class T> class LinkList;
template<class T> class LinkItorator;

// Node
template<class T>
class LinkNode
{
    template<class T> friend class LinkIterator;
    template<class T> friend class LinkList;
private:
    T m_data;
    LinkNode* m_next;
};




// Iterator
template<class T>
class LinkIterator
{
public:
    // default node to null
    LinkIterator()
    {
        m_node = NULL;
    }
    ~LinkIterator(){}
    
    // Assignment operator (=)
    void operator=(LinkNode<T>* node)
    { 
        m_node = node;
    }

    // Dereferencing operator (*)
    T operator*()
    {
        assert(m_node != NULL);
        return m_node -> m_data;
    }
    
    // Incremental operator (++)
    void operator++()
    {
        assert(m_node != NULL);
		m_node = m_node->m_next;
    }

    void operator++(int)
    {
        assert(m_node != NULL);
		m_node = m_node->m_next;
    }

    // Comparison operator (== & !=)
    bool operator==(LinkNode<T>* node)
    {
        return (m_node == node);
    }

    bool operator!=(LinkNode<T>* node)
    {
        return (m_node != node);
    }

private:
    LinkNode<T>* m_node;
};




// List
template<class T>
class LinkList
{
public:
    // Constructor
    LinkList() : m_size(0), m_root(0), m_lastnode(0)
    {

    }

    // Destructor
    ~LinkList()
    {
        // loop through the linklist untill m_root is popped off
        while (m_root)
        {
            Pop();
        }   
    }

    // key linklist positions
    LinkNode<T>* Begin()
    {
        assert(m_root != NULL);
        return m_root;
    }

    LinkNode<T>* End()
    {
        return NULL;
    }

    LinkNode<T>* Last()
    {
        return m_lastnode;
    }




    // linklist operations
    // Push
    void Push(T newData)
    {
        // create the new node first and set it up
        LinkNode<T>* node = new LinkNode<T>;

        assert(node != NULL);
        node -> m_data = newData;
        node -> m_next = NULL;

        // if there is at least 1 node in my list
        if (m_lastnode != NULL)
        {
            m_lastnode -> m_next = node;
            m_lastnode = node;
        }
        else // if list is empty
        {
            m_root = node;
            m_lastnode = node;
        }

        // increment list size
        m_size++;
    }


    // Pop
    void Pop()
    {
        assert(m_root !=NULL);
        
        // if there is only 1 node in the list
        if (m_root -> m_next == NULL)
        {
			delete m_root;             // delete m_root
            m_root = NULL;
        }
        else // more than 1 node in the list
        {
            LinkNode<T>* prevNode = m_root;
            // traverse the list
            while (prevNode -> m_data != NULL && prevNode -> m_next != m_lastnode)
            {
                prevNode = prevNode -> m_next;     
            }
            
            // delete lastnode
            delete m_lastnode;
            prevNode -> m_next = NULL;
            m_lastnode = prevNode;
        }

        // Prevent m_size going negative
        m_size = (m_size == 0 ? m_size : m_size - 1);
    }


    // Pop Front
    void Pop_Front(){
        assert(m_root != NULL);

        // temp node
        LinkNode<T>* temp = m_root;
        
        // root equals next
        m_root = m_root -> m_next;

        if (m_root == NULL)
        {
            m_lastnode = NULL;
        }
        
        // delete temp
        delete temp;

        // Prevent m_size going negative
        m_size = (m_size == 0 ? m_size : m_size - 1);
    }


    // Getter
    int GetSize()
    {
        return m_size;
    }


private:
    int m_size;              // how big is the list
    LinkNode<T>* m_root;     // the first node
    LinkNode<T>* m_lastnode; // the last node
};
