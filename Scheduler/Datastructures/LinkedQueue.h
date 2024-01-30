
/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					-------------
					| item| next | --->
					-------------
General Queue case:

				 frontPtr																backPtr
					\											   						/
					 \											  					   /
					------------- 	  ------------- 	  ------------- 	  -------------
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------

Empty Case:

				 frontptr	 backptr
						\	 /
						 \	/
					---- NULL ------


Single Node Case:
				 frontPtr	 backPtr
					\		/
					 \	   /
					-----------
					|item| next| -->NULL
					-----------

*/

#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

#include "Node.h"
#include "QueueADT.h"
using namespace std;

template <typename T>
class LinkedQueue :public QueueADT<T>
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	LinkedQueue() {
		backPtr = nullptr;
		frontPtr = nullptr;
		count = 0;
	}
	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}
	bool enqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
			frontPtr = newNodePtr; // The queue is empty
		else
			backPtr->setNext(newNodePtr); // The queue was not empty

		backPtr = newNodePtr; // New node is the last node now
		count++;
		return true;
	}

	int getcount() { return count; }

	bool dequeue(T& frntEntry) {
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		count--;

		return true;

	}

	bool peek(T& frntEntry)const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;

	}

	void Print()
	{
		Node<T>* temp = frontPtr;
		while (temp)
		{
			cout << *temp->getItem();
			if (temp->getNext()) {
				cout << " ,";
			}
			temp = temp->getNext();
		}
		cout << endl;
	}

	~LinkedQueue() {
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}

	//copy constructor
	LinkedQueue(const LinkedQueue<T>& LQ)
	{
		Node<T>* NodePtr = LQ.frontPtr;
		if (!NodePtr) //LQ is empty
		{
			frontPtr = backPtr = nullptr;
			return;
		}

		//insert the first node
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		frontPtr = backPtr = ptr;
		NodePtr = NodePtr->getNext();

		//insert remaining nodes
		while (NodePtr)
		{
			Node<T>* ptr = new Node<T>(NodePtr->getItem());
			backPtr->setNext(ptr);
			backPtr = ptr;
			NodePtr = NodePtr->getNext();
		}
	}
};

#endif