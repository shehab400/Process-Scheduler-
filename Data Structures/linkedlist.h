#pragma once
#include"Node.h"
#include<iostream>
using namespace std;
template<class T>
class Linkedlist
{
	Node<T>* Head;
	int count;

public:
	Linkedlist()
	{
		count = 0;
		Head = nullptr;
	}

	void PrintList()	const
	{
		cout << "\nprinting list contents:\n\n";
		Node<T>* p = Head;
		while (p)
		{
			cout << "[ " << p->getitem() << " ]";
			cout << "--->";
			p = p->getnext();
		}
		cout << "NULL\n";
	}
	bool isempty()
	{
		if (Head == nullptr)
		{
			return true;
		}
		return false;
	}
	bool add(T item)
	{
		Node<T>* p = new Node<T>;
		Node<T>* temp = Head;
		if (!Head)
		{
			Head = p;
			Head->setitem(item);
			count++;
		
			return true;
		}
		else
		{
			while (temp->getnext())
			{
				temp = temp->getnext();
			}
			temp->setnext(p);
			p->setitem(item);
			count++;
			return true;
		}
		return false;

	}
	Node<T>* Returnhead()
	{
		return Head;
	}
	void deletefirst( T & newitem)
	{
		if (isempty())
		{
			return;
		}
		Node<T>* temp = Head;
		newitem = Head->getitem();
		Head = Head->getnext();
		count--;
	
		delete temp;
	}
	void deletehead()
	{
		if (isempty())
		{
			return;
		}
		Node<T>* temp = Head;
		
		Head = Head->getnext();
		count--;

		delete temp;
	}
	void deletenode(T item)
	{
		if (isempty())
		{
			return;
		}
		if (Head->getitem()==item)
		{
			deletefirst(item);
			count--;
			return;
		}
		Node<T>* back = Head;
		Node<T>* nodetodelete = Head->getnext();
		while (nodetodelete)
		{
			if (nodetodelete->getitem()==item)
			{
				back->setnext(nodetodelete->getnext());
				delete nodetodelete;
				count--;
				return;
			}
			back = back->getnext();
			nodetodelete = nodetodelete->getnext();
		}
	}
	int getcount()
	{
		return count;
	}
	void DeleteAll()
	{
		Node<T>* P =Head;
		while (Head)
		{
			P = Head->getnext();
			delete Head;
			Head = P;
		}
	}
};