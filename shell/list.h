#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int LIST_TYPE;
typedef struct List List;
typedef struct Node Node;

struct Node {
	LIST_TYPE value;
	Node* next;
	Node* prev;
};

struct List{
	int size;
	Node* head;
	Node* tail;
	void (*list_print_function)(void*);
};

Node* node_constructor(LIST_TYPE value);
List* list_constructor();
void list_append(LIST_TYPE value, List* list);
void list_insert(int index, LIST_TYPE value, List *list);
void list_prepend(LIST_TYPE value, List* list);
void list_remove(int index, List *list);
LIST_TYPE list_get(int index, List *list);
void list_removeFirst(List *list);
void list_removeLast(List *list);

#endif
