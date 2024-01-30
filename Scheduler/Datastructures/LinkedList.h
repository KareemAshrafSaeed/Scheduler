#ifndef LINKED_LIST_
#define LINKED_LIST_
#include<iostream>
using namespace std;
#include"Node.h"
template<class T>
class LinkedList
{
private:
	Node<T>* head;
	Node<T>* tail;
	int count;
public:
	LinkedList() {
		head = nullptr;
		tail = nullptr;
		count = 0;
	}
	//Copy Constructor
	LinkedList(const LinkedList& v) :head(nullptr), tail(nullptr)
	{
		Node<T>* p1 = nullptr;//current
		Node<T>* p2 = nullptr;//next

		if (v.gethead() == nullptr)
			head = nullptr;
		else
		{
			head = new Node<T>;
			head->setItem(v.gethead()->getItem());
			p1 = head;
			p2 = v.gethead()->getNext();
		}
		while (p2)
		{
			Node<T>* temp = new Node<T>;
			temp->setItem(p2->getItem());
			p1->setNext(temp);
			p1 = p1->getNext();
			p2 = p2->getNext();
		}
		if (tail) {
			p1->setNext(nullptr);
			tail = p1;
		}
	}
	int getcount() { return count; }
	Node<T>* gethead() const {
		return head;
	}
	Node<T>* gettail() const {
		return tail;
	}
	void Inserthead(T value) {

		Node<T>* temp = new Node<T>(value);
		temp->setNext(head);
		head = temp;
		if (!tail) {
			tail = head;
		}
		count++;
	}
	void InsertEnd(T value) {

		if (!head) {
			Inserthead(value);
		}
		else
		{
			Node<T>* add = new Node<T>(value);
			tail->setNext(add);
			tail = add;
			count++;
		}
	}

	bool Find(T value) {
		Node<T>* temp = head;
		while (temp) {
			if (temp->getItem() == value)
				return true;
			temp = temp->getNext();
		}
		return false;
	}

	bool FindPointer(T value, T &result) {
		Node<T>* temp = head;
		while (temp) {
			if (*(temp->getItem()) == *value) {
				result = temp->getItem();
				return true;
			}
			temp = temp->getNext();
		}
		result = nullptr;
		return false;
	}
	int CountOccurance(T value) {
		Node<T>* temp = head;
		int num = 0;
		while (temp != NULL) {
			if (temp->getItem() == value)
				num++;
			temp = temp->getNext();
		}
		return num;
	}
	void DeleteFirst() {
		Node<T>* temp = head;
		head = head->getNext();
		if (count == 1) {
			tail = nullptr;
		}
		temp->setNext(NULL);
		delete temp;
		count--;
	}
	void DeleteLast() {
		Node<T>* curr = head;
		Node<T>* prev = NULL;
		while (curr != tail) {
			prev = curr;
			curr = curr->getNext();
		}
		tail = prev;
		tail->setNext(NULL);
		delete curr;
		count--;
	}
	void insert(T value, Node<T>* Toinsert) {

		if (Toinsert == head) {
			Inserthead(value);
		}
		else if (Toinsert == tail)
		{
			InsertEnd(value);
		}
		else
		{
			Node<T>* add = new Node<T>(value);
			add->setNext(Toinsert->getNext());
			Toinsert->setNext(add);
			count++;
		}
	}

	bool deleteNodewithptr(Node<T>* Dele) {

		Node<T>* curr = head;
		Node<T>* prev = NULL;
		if (head == Dele) {
			DeleteFirst();
			return true;
		}
		while (curr) {
			if (curr == Dele)
			{
				prev->setNext(curr->getNext());
				if (curr == tail) {
					tail = prev;
				}
				curr->setNext(NULL);
				delete curr;

				count--;
				return true;
			} else {
				prev = curr;
				curr = curr->getNext();
			}
		}
		return false;
	}
	bool DeleteNode(T value) {
		if (!Find(value))
			return false;
		Node<T>* curr = head;
		Node<T>* prev = NULL;
		if (head->getItem() == value) {
			DeleteFirst();
			return true;
		}
		while (curr) {
			 if (curr->getItem() == value)
			{
				prev->setNext(curr->getNext());
				if (curr == tail) {
					tail = prev;
				}
				curr->setNext(NULL);
				delete curr;
				
				count--;
				return true;
			}
			else {
				prev = curr;
				curr = curr->getNext();
			}
		}

		return false;
	}
	bool DeleteNodes(T value) {
		if (!Find(value))
			return false;
		Node<T>* temp = head;
		int i = 0;
		while (i != count) {
			DeleteNode(value);
			i++;
		}
		return true;
	}

	void Print() const
	{
		Node<T>* p = head;
		while (p)
		{
			cout << *p->getItem();
			if (p->getNext()) {
				cout << ",";
			}
			p = p->getNext();
		}
		cout << endl;
	}
	void DeleteAll()
	{
		Node<T>* P = head;
		while (head)
		{
			P = head->getNext();
			delete head;
			head = P;
		}
	}
	~LinkedList() {
		DeleteAll();
	}

};
#endif