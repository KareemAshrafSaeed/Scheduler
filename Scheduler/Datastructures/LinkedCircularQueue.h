#include "Node.h"
#include "QueueADT.h"
#include <iostream>
using namespace std;

template <typename T>
class LinkedCircularQueue :public QueueADT<T>
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	LinkedCircularQueue() {
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
		if (isEmpty()) {	//special case if this is the first node to insert
			frontPtr = newNodePtr; // The queue is empty
			backPtr = newNodePtr;
		} else {
			backPtr->setNext(newNodePtr);
		}
		newNodePtr->setNext(frontPtr);
		backPtr = newNodePtr; // New node is the last node now
		count++;
		return true;
	}
	bool dequeue(T& frntEntry) {
		if (isEmpty()) {
			frntEntry = nullptr;
			return false;
		}
		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		backPtr->setNext(frontPtr);
		nodeToDeletePtr->setNext(nullptr);
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr) { // Special case: last node in the queue
			backPtr = nullptr;
			frontPtr = nullptr;
		}

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		count--;

		return true;
	}

	int getcount() { return count; }
	bool peek(T& frntEntry)const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;

	}
	void Print() {
		Node<T>* temp = frontPtr;
		if (!frontPtr) {
			cout << endl;
			return;
		}
		if (temp->getNext() == frontPtr) {
			cout << *temp->getItem()<<endl;
			return;
		}
		while (temp->getNext() != frontPtr)
		{
			cout << *temp->getItem();
			if(temp) cout << ",";
			temp = temp->getNext();
			if (temp->getNext() == frontPtr) {
				cout << *temp->getItem();
			}
		}
		cout << endl;
	}

	~LinkedCircularQueue() {
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}

	//copy constructor
	LinkedCircularQueue(const LinkedCircularQueue<T>& LQ)
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

	friend ostream& operator<<(ostream& os, const LinkedCircularQueue<T>& p)
	{
		os << p.Print();
		return os;
	}
};