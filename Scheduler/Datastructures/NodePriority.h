
#ifndef _NODE_PRIORITY
#define _NODE_PRIORITY
template < typename T>
class NodePriority
{
private:
	T item; // A data item
	NodePriority<T>* next; // Pointer to next node
	int priority;
public:
	NodePriority();
	NodePriority(const T& r_Item);
	NodePriority(const T& r_Item, NodePriority<T>* nextNodePtr);
	NodePriority(const T& r_Item, NodePriority<T>* nextNodePtr,int p);
	void setItem(const T& r_Item);
	void setNext(NodePriority<T>* nextNodePtr);
	void setPriority(int P);
	int getPriority();
	T getItem() const;
	NodePriority<T>* getNext() const;
}; // end Node
#endif

template < typename T>
NodePriority<T>::NodePriority()
{
	priority = 0;
	next = nullptr;
}

template < typename T>
NodePriority<T>::NodePriority(const T& r_Item)
{
	item = r_Item;
	next = nullptr;
	priority = 0;
}

template < typename T>
NodePriority<T>::NodePriority(const T& r_Item, NodePriority<T>* nextNodePtr)
{
	priority = 0;
	item = r_Item;
	next = nextNodePtr;
}

template < typename T>
NodePriority<T>::NodePriority(const T& r_Item, NodePriority<T>* nextNodePtr, int p)
{
	item = r_Item;
	next = nextNodePtr;
	setPriority(p);
}
template < typename T>
void NodePriority<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void NodePriority<T>::setNext(NodePriority<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T NodePriority<T>::getItem() const
{
	return item;
}

template < typename T>
NodePriority<T>* NodePriority<T>::getNext() const
{
	return next;
}

template<typename T>
int NodePriority<T>::getPriority()
{
	return priority;
}
template<typename T>
void NodePriority<T>::setPriority(int P)
{
	if (P > 0)
	{
		priority = P;
		return;
	}
	priority = 0;
	return;
}
