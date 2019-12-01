#pragma once
#include <cassert>

template <class T>
class LinkIterator;
template <class T>
class LinkList;


// LinkNode
template <class T>
class LinkNode
{
	friend class LinkIterator<T>;
	friend class LinkList<T>;

private:
	T m_data;
	LinkNode *m_next;
	LinkNode *m_previous;
	int priority;
};



// Iterator
template <class T>
class LinkIterator
{
public:
	LinkIterator()
	{
		m_node = NULL;
	}
	~LinkIterator()
	{
	}
	// Assignment operator (=)
	void operator=(LinkNode<T> *node)
	{
		m_node = node;
	}
	// Dereferencing operator (*)
	T &operator*()
	{
		assert(m_node != NULL);
		return m_node->m_data;
	}
	// Incremental operators (++) -- Prefix and Postfix
	void operator++() // Prefix
	{
		assert(m_node != NULL);
		m_node = m_node->m_next;
	}
	void operator++(int) // Postfix
	{
		assert(m_node != NULL);
		m_node = m_node->m_next;
	}
	// Decremental operators (--) -- Prefix and Postfix
	void operator--() // Prefix
	{
		assert(m_node != NULL);
		m_node = m_node->m_previous;
	}
	void operator--(int)
	{
		assert(m_node != NULL);
		m_node = m_node->m_previous;
	}
	// Comparison operators (!= & ==)
	bool operator!=(LinkNode<T> *node)
	{
		return (m_node != node);
	}
	bool operator==(LinkNode<T> *node)
	{
		return (m_node == node);
	}
	LinkNode<T> *m_node; 
private:
};





















// Link list
template <class T>
class LinkList
{
public:
	LinkList() : m_size(0), m_root(0), m_lastNode(0)
	{
	}
	~LinkList()
	{

	}


	// returns the first node 
	LinkNode<T> *Begin()
	{
		assert(m_root != NULL);
		return m_root;
	}
	// returns NULL
	LinkNode<T> *End()
	{
		return NULL;
	}
	// returns the last node
	LinkNode<T> *Last()
	{
		return m_lastNode;
	}
	LinkNode<T> *TargetNode(int val)
	{
		LinkIterator<T> it;
		LinkNode<T> *temp = m_root;

		if (m_root == NULL)
		{   // if list is empty
			return m_root;
		}
		
		if (m_root->m_next == NULL)
		{
			// if there is only 1 element
			return m_root;
		}
		else
		{   // if there is more nodes
			it = m_root;
			if (it.m_node->m_next==NULL)
			{
				return m_root;
			}
			else
			{
				for (int i = 0; i < m_size-1; i++)
				{
					if (it.m_node->m_next->priority == val && it.m_node->m_next->m_next->priority != val)
					{
						return it.m_node;
						m_root = temp;
						break;
					}
					else
					{
						if (it.m_node->m_next == NULL)
						{
							return it.m_node;
							m_root = temp;
							break;
						}
						else
						{
							it++;
						}
					}
				}
				return it.m_node;
				m_root = temp;
			}		
		}		
	}

	// Pop front
	void Pop()
	{
		assert(m_root != NULL);

		LinkNode<T> *temp = m_root;

		m_root = m_root->m_next;

		if (m_root != NULL)
		{
			m_root->m_previous = NULL;
		}
		else 
		{
			m_lastNode = NULL;
		}

		m_root = temp;
		delete temp;
		temp = NULL;

		m_size = (m_size == 0 ? m_size : m_size - 1);
	}


	void Push(T newData, int pio)
	{
		LinkNode<T> *node = new LinkNode<T>;

		assert(node != NULL);
		node->m_data = newData;
		node->m_next = NULL;
		node->m_previous = NULL;
		node->priority = pio;

		LinkNode<T> *temp = m_root;

		LinkNode<T> *targetnode = TargetNode(pio);
		
		if (m_size==0)
		{
			m_root = node;
			m_lastNode = node;
		}
		

		if (targetnode != NULL) 
		{	// only 2 node
			if (targetnode->m_data == m_lastNode->m_data)
			{
				targetnode->m_next = node;
				node->m_previous = targetnode;
				m_lastNode = node;
			}
			else if (m_lastNode->m_previous==targetnode)
			{
				targetnode->m_next = node;
				node->m_previous = targetnode;
				m_lastNode->m_previous = node;
			}
			else
			{	// muiltiple nodes
				targetnode->m_next->m_next->m_previous = node; 
				node->m_previous = targetnode;
				targetnode->m_next = node;
			}
			m_root = temp;
		}
		else 
		{	// only 1 node
			node->m_previous = m_root;
			m_root->m_next = node;
			m_lastNode = node;
		}


		m_size++;
	}
	
	int GetSize()
	{
		return m_size;
	}


private:
	int m_size;				 // Size 
	LinkNode<T> *m_root;	 // Points to the root 
	LinkNode<T> *m_lastNode; // Points to the last node 
};