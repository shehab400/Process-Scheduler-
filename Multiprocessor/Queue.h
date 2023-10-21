#pragma once
#include"../Data Structures/Node.h"
#include<iostream>
template<class T>
class Queue
{
	Node<T>* front;
	Node<T>* back;
	int count=0;

public:
	Queue()
	{
		front = nullptr;
		back = nullptr;
	}
	void deletefirst(T& newitem)
	{
		if (isempty())
		{
			return;
		}
		Node<T>* temp = front;
		newitem = front->getitem();
		front = front->getnext();
		count--;

		delete temp;
	}
	bool isempty()
	{
		if (!front)
		{
			return true;
		}
		return false;
	}
	bool enqueue(T newitem)
	{
		Node<T>* p = new Node<T>;
		Node<T>* temp = new Node<T>;
		if (isempty())
		{
			p->setitem(newitem);
			front = p;
			back = p;
			count++;
			return true;
		}
		p->setitem(newitem);
		temp = back;
		back = p;
		temp->setnext(back);
		count++;
		return true;

	}
	bool dequeue(T& item)
	{
		if (isempty())
			return false;
		Node<T>* deletenode = front;
		item = front->getitem();
		front = front->getnext();
		if (!deletenode)
		{
			back = nullptr;
		}
		delete deletenode;
		count--;
		return true;
	}
	T peek()
	{
		if (isempty())
		{
			return NULL;
		}
		return front->getitem();
	}
	void printqueue()
	{
		Node<T>* p = front;

		if (isempty())
		{
			cout << "queue is empty";
		}
		while (p)
		{
			cout << p->getitem() << " ";
			p = p->getnext();
		}
	}
	Node<T>* returnfront()
	{
		return front;

	}
	int getcount()
	{
		return count;
	}
	void sethead(T ITEM)
	{
		front->setitem(ITEM);
	}

};
