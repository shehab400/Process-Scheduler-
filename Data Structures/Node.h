#pragma once
template<class T>
class Node
{
	Node<T>* next;
	T item;
	int priority;
public:
	Node()
	{
		next = nullptr;
	}
	Node(T newitem)
	{
		item = newitem;
		next = nullptr;
	}
	void setitem(T newitem)
	{
		item = newitem;
	}
	void setnext(Node<T>* p)
	{
		next = p;
	}
	Node<T>* getnext()
	{
		
			return next;
		
		
	}
	T getitem()
	{
		return item;
	}
	int getpriority()
	{
		return priority;
	}
	void setpriorty(int p)
	{
		priority = p;
	}
};