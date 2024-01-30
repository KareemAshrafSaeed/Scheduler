
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

#ifndef LINKED_PRIORITY_QUEUE_
#define LINKED_PRIORIRY_QUEUE_

#include "NodePriority.h"
#include <vector>
using namespace std;


template <typename T>
class LinkedPriorityQueue
{
private:

	NodePriority<T>* backPtr;
	NodePriority<T>* frontPtr;
	int count;
public:
	LinkedPriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, int Priority);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	void Print();
	int gatcount();
	~LinkedPriorityQueue();

	//copy constructor
	LinkedPriorityQueue(const LinkedPriorityQueue<T>& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;
	count = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool LinkedPriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

template <typename T>
int LinkedPriorityQueue<T>::gatcount() { return count; }
/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool LinkedPriorityQueue<T>::enqueue(const T& newEntry, int priority)
{
	NodePriority<T> * temp, * curr, * pre = NULL;
	temp = new NodePriority<T>(newEntry);
	temp->setPriority(priority);
	if (frontPtr == nullptr || priority > frontPtr->getPriority())
	{
		temp->setNext(frontPtr);
		frontPtr = temp;
		count++;
		return true;
	}
	curr = frontPtr;
	while (curr && priority <= curr->getPriority())
	{
		pre = curr;
		curr = curr->getNext();
	}
	temp->setNext(pre->getNext());
	pre->setNext(temp);
	count++;
	return true;

	
	/*
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	return true;
	*/
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool LinkedPriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	NodePriority<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	nodeToDeletePtr->setNext(nullptr);
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;
	count--;
	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
template <typename T>
bool LinkedPriorityQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}

template <typename T>
void LinkedPriorityQueue<T>::Print()
{
	NodePriority<T>* temp = frontPtr;
	while (temp)
	{
		cout << *temp->getItem();
		temp = temp->getNext();
		if(temp) cout << ",";
	}
	cout << endl;
}
///////////////////////////////////////////////////////////////////////////////////
/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
template <typename T>
LinkedPriorityQueue<T>::~LinkedPriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: Copy constructor
To avoid shallow copy,
copy constructor is provided

Input: LinkedQueue<T>: The Queue to be copied
Output: none
*/

template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue(const LinkedPriorityQueue<T>& LQ)
{
	NodePriority<T>* NodePtr = LQ.frontPtr;
	LQ.count = count;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	NodePriority<T>* ptr = new NodePriority<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		NodePriority<T>* ptr = new NodePriority<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
}
#endif