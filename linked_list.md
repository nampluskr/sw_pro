## Linked List (head, tail)

```cpp
#include <iostream>

using namespace std;

struct Data {
	int idx, value;
};

struct LinkedList {
	struct Node {
		Data data;
		Node* next;
	};
	Node* head;
	Node* tail;

	void init() { head = NULL; tail = NULL; }
	void push_front(const Data& data) {
		Node* temp = new Node({data, NULL});

		if (head == NULL) { head = temp; tail = temp; }
		else { temp->next = head; head = temp; }
	}
	void push_back(const Data& data) {
		Node* temp = new Node({data, NULL});

		if (head == NULL) { head = temp; tail = temp; }
		else { tail->next = temp; tail = temp; }
	}
	void insert(Node* node, const Data& data) {
		Node* temp = new Node;
		temp->data = data;
		temp->next = node->next;
		node->next = temp;
	}
	void erase(Node* node) {
		Node* temp = node->next;
		node->next = temp->next;
		delete temp;
	}

};

void display(const LinkedList& list)
{
	//LinkedList::Node* node;
	for (auto node = list.head; node != NULL; node = node->next) {
		printf("(%d, %d) - ", node->data.idx, node->data.value);
	}
	printf("NULL\n");
}

//void display(const LinkedList& list)
//{
//	LinkedList::Node* node = list.head;
//	while (node != NULL) {
//		printf("(%d, %d) - ", node->data.idx, node->data.value);
//		node = node->next;
//	}
//	printf("NULL\n");
//}

LinkedList list;

//메인 함수
int main()
{
	list.init();
	display(list);

	list.push_back({ 1, 10 });
	list.push_back({ 2, 20 });
	list.push_back({ 3, 30 });
	display(list);

	list.push_front({ 40, 40 });
	list.push_front({ 50, 50 });
	display(list);

	list.insert(list.head->next->next, { 6, 60 });
	display(list);

	// 세번째 노드를 삭제
	list.erase(list.head);
	display(list);

	return 0;
}
```
