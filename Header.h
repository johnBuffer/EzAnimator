#pragma once

#include <iostream>
#include <string>
using namespace std;


template <typename T>
class DynamicQueue
{
private:
	// Structure for the queue nodes
	struct QueueNode
	{
		T value;       // Value in a node
		QueueNode *next;    // Pointer to the next node
	};
	QueueNode *top;      // The top of the queue
	QueueNode *bottom;     // The bottom of the queue
	int numItems;          // Number of items in the queue

public:
	DynamicQueue() {
		top = nullptr;
		bottom = nullptr;
		numItems = 0;
	}

	~DynamicQueue() {
		delete[] top;
		delete[] bottom;
	}

	template<typename... Args>
	void enqueue(Args&&... args);

	bool isEmpty() const {
		bool empty;
		if (numItems > 0)
			empty = false;
		else
			empty = true;
	}

	void display() const {
		QueueNode* temp = top;
		while (temp != nullptr) {
			cout << temp->value << endl;
			temp = temp->next;
		}
	}

	void clear();
};


template<class T>
void DynamicQueue<T>::clear()
{
	T temp;
	while (!isEmpty()) {
		//dequeue(data);
	}
}

template<typename T>
template<typename... Args>
void DynamicQueue<T>::enqueue(Args&&... args)
{
	// Create new node
	QueueNode* new_node = new QueueNode();
	bottom->next = new_node;

	new (&(new_node->value)) T(args...);
	bottom = new_node;
}