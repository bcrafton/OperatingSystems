#include "list.h"

Node* node_constructor(LIST_TYPE value){
	Node *node = malloc(sizeof(Node));
	node->next = NULL;
	node->prev = NULL;
	node->value = value;
	return node;
}

List* list_constructor(){
	List *list = malloc(sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->list_print_function = NULL;
	return list;
}

void list_append(LIST_TYPE value, List* list){
	Node* newNode = node_constructor(value);
	if(list->head == NULL && list->tail == NULL){
		list->head = newNode;
		list->tail = newNode;
	}
	else{
		list->tail->next = newNode;
		newNode->prev = list->tail;
		list->tail = newNode;
	}
	list->size++;
}

void list_prepend(LIST_TYPE value, List* list){
	Node* newNode = node_constructor(value);
	if(list->head == NULL && list->tail == NULL){
		list->head = newNode;
		list->tail = newNode;
	}
	else{
		list->head->prev = newNode;
		newNode->next = list->head;
		list->head = newNode;
	}
	list->size++;
}

void list_set(int index, LIST_TYPE value, List *list){
	if(index >= list->size || index < 0){
		printf("index is out of bounds.\n");
		assert(0);
		return;
	}
	int counter = 0;
	Node* ptr;
	for(ptr = list->head; ptr != NULL; ptr = ptr->next){
		if(counter == index){
			ptr->value = value;
			return;
		}
		counter++;
	}
}

void list_insert(int index, LIST_TYPE value, List *list){
	if(index > list->size || index < 0){
		printf("index is out of bounds.\n");
		assert(0);
		return;
	}
	Node* newNode = node_constructor(value);
	// this wud break if it wasnt the case that the only way to insert something to a empty list is to insert it into the 0 index. 
	// whcih means prepend will handle this fine.
	if(index == 0){
		list_prepend(value, list);
	}
	else if(index == list->size){
		list_append(value, list);
	}
	else{
		int counter = 0;
		Node* ptr;
		for(ptr = list->head; ptr != NULL; ptr = ptr->next){
			if(counter == index){
				Node* left = ptr->prev;
				Node* right = ptr;
				left->next = newNode;
				newNode->prev = left;
				right->prev = newNode;
				newNode->next = right;
				list->size++;
				return;
			}
			counter++;
		}
	}
}

void list_removeFirst(List *list){
	Node* newHead = list->head->next;
	Node* oldHead = list->head;
	newHead->prev = NULL;
	oldHead->next = NULL;
	list->head = newHead;
	list->size--;
}
void list_removeLast(List *list){
	Node* newTail = list->tail->prev;
	Node* oldTail = list->tail;
	newTail->next = NULL;
	oldTail->prev = NULL;
	list->tail = newTail;
	list->size--;
}

void list_remove(int index, List *list){
	if(index >= list->size || index < 0){
		printf("index is out of bounds.\n");
		assert(0);
		return;
	}
	if(list->size == 1){
		list->head == NULL;
		list->tail == NULL;
		list->size--;
		return;
	}
	if(index == 0){
		list_removeFirst(list);
	}
	else if(index == list->size-1){
		list_removeLast(list);
	}
	else{
		int counter = 0;
		Node* ptr;
		for(ptr = list->head; ptr != NULL; ptr = ptr->next){
			if(counter == index){
				Node* left = ptr->prev;
				Node* right = ptr->next;
				left->next = right;
				right->prev = left;
				list->size--;
				return;
			}
			counter++;
		}
	}
}

LIST_TYPE list_get(int index, List *list){
	int counter = 0;
	Node* ptr;
	for(ptr = list->head; ptr != NULL; ptr = ptr->next){
		if(counter == index){
			return ptr->value;
		}
		counter++;
	}
	return NULL;
}

