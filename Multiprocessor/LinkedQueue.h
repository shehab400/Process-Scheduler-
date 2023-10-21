#pragma once
#include"../Data Structures/linkedlist.h"
#include"../Process/Process.h"
#include"../Data Structures/Node.h"
#include<iostream>
using namespace std;
template<class T>
class Linkedqueue 
{
	Node<T>* front;
	int count = 0;
public:
	Linkedqueue()
	{
		front = nullptr;

	}
	bool isempty()
	{
		if (!front)
		{
			return true;
		}
		return false;
	}
	void enqueue(T newitem, int p)
	{
	
		Node<T>* node = new Node<T>;
		node->setitem(newitem);
		node->setpriorty(p);
		node->setnext(nullptr);
		if (front==nullptr)
		{
			front = node;
			count++;
			return;
		}
		if (p<(front->getpriority()))
		{
			node->setnext(front);
			front = node;
			count++;
			return;
		}
		Node<T>*temp = front;
		while ((temp->getnext()!=nullptr)&&((temp->getnext()->getpriority())<p))
		{
			temp = temp->getnext();
		}
		node->setnext(temp->getnext());
		temp->setnext(node);
		count++;
	}
	bool dequeue(T&item)
	{
		if (isempty())
			return false;
		Node<T>* deletenode = front;
		item= front->getitem();
		front = front->getnext();
		delete deletenode;
		count--;
		return true;
	}
	void peek(T&item)
	{
		if (isempty())
		{
			return;
		}
	item= front->getitem();
	}
	Node<T>* returnfront()
	{
		return front;

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
	int getcount()
	{
		return count;
	}
};