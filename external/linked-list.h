/*
Created by: andiputraw

First iteration on stb style linked list library.

It include not only standard linked list operation (append, remove, searching). But also include sorting
and travelsal using iterator pattern to loop through a list cleanly see example for more.

Option
#define LINKED_LIST_TYPE <your type>

this macro will replace the type for NodeInfoType.
it will default on `int` type
You might also want to define LINKED_LIST_NO_INT_OPERATION to disable the int related function

#define LINKED_LIST_IMPLEMENTATION

this macro will append the implementation on the source files. each project only need to implement once.

Example:
```c
#define LINKED_LIST_IMPLEMENTATION
#include "linked_list.h"

void print_node(int value) {
    printf("%d -> ", value);
}

int main() {
    NodeAddress head = NULL;
    
    node_append(&head, 10);
    node_append(&head, 20);
    
    node_show(head, print_node);

    return 0;
}
  ```

 copying a list if the value more than 85
 ```c
    ...
    while (current = node_iter_next(&iter))
    {
        if(current->info->nilai_uts > 85 ) {
            // you probably want to copy the info if you use abstract data type.
            node_prepend(&L2.first, current->info);
        }
    }
    ...
 ```
 */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef LINKED_LIST_TYPE
#define LINKED_LIST_TYPE void*
#endif

typedef LINKED_LIST_TYPE NodeInfoType;
typedef struct NodeT* NodeAddress;
typedef bool (* LinkedListComparisonFn) (NodeInfoType n1, NodeInfoType n2);
typedef void (* LinkedListPrintFn) (NodeInfoType node);
typedef void (* LinkedListInfoDestroyFn) (NodeInfoType node);

typedef struct NodeT {
    NodeInfoType info;
    NodeAddress next;
} Node;

typedef struct NodeIterator {
    NodeAddress cur;
} NodeIterator;

// Allocate node in the heap
void node_create(NodeAddress* node);
// Initialize node with value
void node_init(NodeAddress* node, NodeInfoType value);
// Print the node baed on Print function
void node_show(NodeAddress head, LinkedListPrintFn fn);
// Append node on the front
void node_prepend(NodeAddress* head, NodeInfoType value);
// append node on the back
void node_append(NodeAddress* head, NodeInfoType value);
// insert node at index
void node_insert_at(NodeAddress* head, NodeInfoType value, int at);
// insert node sorted sequentialy based on comparison function
void node_insert_sorted(NodeAddress* head, NodeInfoType value, LinkedListComparisonFn comparison_fn) ;
// find node based on comparison function
NodeAddress node_find(NodeAddress head, NodeInfoType value,  LinkedListComparisonFn comparison_fn);
// delete first node
NodeInfoType node_remove_first(NodeAddress* head);
// delete last node
NodeInfoType node_remove_last(NodeAddress* head);
// remove node on index
NodeInfoType node_remove_at(NodeAddress* head, int at);
// get at index
NodeInfoType node_at(NodeAddress head, int index );
// reverse node
void node_reverse(NodeAddress* head);
// count node
int node_count(NodeAddress head);
// sort node based on comparison funcition
void node_sort(NodeAddress* head, LinkedListComparisonFn comparison_fn);
void node_swap(NodeAddress pre, NodeAddress n1, NodeAddress n2);
// internal function
bool node__safe__compare(NodeAddress n1, NodeAddress n2, LinkedListComparisonFn comp_fn);

// Create iterator struct
NodeIterator node_iter_init(NodeAddress start);
// get next on interator, return NULL on end
NodeAddress node_iter_next(NodeIterator *iter);
// check if the iterator end
bool node_iter_is_end(NodeIterator iter);


#ifdef LINKED_LIST_IMPLEMENTATION

void node_create(NodeAddress* node){
    NodeAddress new_node = (NodeAddress)malloc(sizeof(Node));
    *node = new_node;
}
void node_init(NodeAddress* node, NodeInfoType value){
    (*node)->info = value;
    (*node)->next = NULL;
}
void node_show(NodeAddress head, LinkedListPrintFn print_fn){
    if(head == NULL) {
        printf("NULL\n");
        return;
    }
    print_fn(head->info);
    node_show(head->next, print_fn);
}
void node_prepend(NodeAddress* head, NodeInfoType value){
    NodeAddress new_node;
    node_create(&new_node);

    node_init(&new_node, value);

    new_node->next = *head;
    (*head) = new_node;
}
void node_append(NodeAddress* head, NodeInfoType value){
    NodeAddress new_node;
    node_create(&new_node);

    node_init(&new_node, value);

    if (*head == NULL) {
        *head = new_node;
    }else {
        NodeAddress cur = *head;
    
        while(cur->next != NULL) {
            cur = cur->next;
        }
    
        cur->next = new_node;
    }

}
void node_insert_at(NodeAddress* head, NodeInfoType value, int at){
    NodeAddress new_node;
    node_create(&new_node);

    node_init(&new_node, value);
    // 3 -> 2 -> 1
    //      2 -> 4 -> 1
    //      ^
    
    if(at == 0) {
        node_prepend(head, value);
        return;
    }

    NodeAddress cur = *head;
    
    for(int i = 0; i < at - 1; i++) {
        if (cur == NULL) {
            printf("ERROR(node_insert_at): panjang node hanya %d\n", i);
            return;
        }

        cur = cur->next;
    }

    new_node->next = cur->next;
    cur->next = new_node;
}
NodeAddress node_find(NodeAddress head, NodeInfoType value, LinkedListComparisonFn comparison_fn){
    
    if(head == NULL) {
        return NULL;
    }

    NodeAddress cur = head;

    do{
        if(comparison_fn(cur->info, value)){
            break;
        }
        cur = cur->next;
    }while(cur != NULL);

    return cur;
}
NodeInfoType node_remove_first(NodeAddress* head){
    NodeInfoType info = {0};
    NodeAddress cur = *head;
    if(head == NULL) {
        printf("ERROR(node_remove_first): linked list kosong\n");
        return info;
    }
    info = cur->info; 
    (*head) = cur->next;
    
    free(cur);
    return info;
}
NodeInfoType node_remove_last(NodeAddress* head){
    NodeInfoType info;
    if(head == NULL) {
        printf("ERROR(node_remove_last): linked list kosong\n");
        return info;
    }

    if((*head)->next == NULL) {
        info = node_remove_first(head);
        return info;
    }
    // 3 -> 2 -> 1 -> NULL
    //      ^    ^
    // 3 -> 2 -> NULL
    //      ^    ^

    NodeAddress cur = *head;
    NodeAddress prev = NULL;
    while(cur->next != NULL) {
        prev = cur;
        cur = cur->next;
    }
    info = cur->info;
    prev->next = NULL;
    free(cur);

    return info;
}
NodeInfoType node_remove_at(NodeAddress* head, int at){
    NodeInfoType info;
    if(at == 0) {
        info = node_remove_first(head);
        return info;
    }

    NodeAddress cur = *head;
    NodeAddress prev = NULL;

    for(int i = 0; i < at; i++) {
        if (cur == NULL) {
            printf("ERROR(node_insert_at): panjang node hanya %d\n", i);
            return info;
        }
        prev = cur;
        cur = cur->next;
    }

    // 3 -> 2 -> 1 -> NULL
    // ^    ^
    // 3 -> 1 -> NULL
    // ^
    info = cur->info;
    prev->next = cur->next;
    free(cur);
    return info;
}
void node_reverse(NodeAddress* head){
    NodeAddress cur = *head;
    NodeAddress prev = NULL;
    NodeAddress tmp;

    // 2 -> 1 | 1 -> NULL
    // NULL | 3 -> 2 | 3 -> NULL | 2 -> 1 | 2 -> 3
    // ? | NULL | 3 -> NULL

    // 3 -> 2 -> 1 -> NULL
    // 
    // 3 -> 1 -> NULL
    // ^

    while(cur->next != NULL) {
        tmp = prev;
        prev = cur;
        cur = cur->next;

        prev->next = tmp;
    }
    cur->next = prev;
    *head = cur;
}

int node_count(NodeAddress head) {
    NodeAddress cur = head;
    int count;
    while(cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}

void node_sort(NodeAddress* head, LinkedListComparisonFn comparison_fn) {
    if ((*head) == NULL) {
        return;
    }

    int swapped;
    Node *cur;
    

    do {
        swapped = false;
        cur = *head;

        while(cur->next != NULL) {
            if( node__safe__compare(cur, cur->next, comparison_fn)){
                NodeInfoType tmp = cur->info;
                cur->info = cur->next->info;
                cur->next->info = tmp;
                swapped = true;
            }
            cur = cur->next;
        }

    }while(swapped);
}

void node_swap(NodeAddress pre, NodeAddress n1, NodeAddress n2) {
    pre->next = n2;
    n2->next = n1;
    n1->next = n2->next;
}

void node_insert_sorted(NodeAddress* head, NodeInfoType value, LinkedListComparisonFn comparison_fn) {
    NodeAddress cur, pre;
    NodeAddress new_node;
    cur = *head;
    pre = NULL;
    node_create(&new_node);
    node_init(&new_node, value);

    while(cur != NULL) {
        if(node__safe__compare(new_node, cur, comparison_fn)) {
            break;
        }
        pre = cur;
        cur = cur->next;
    }

    if(pre == NULL) {
        *head = new_node;
        new_node->next = cur;
    }else {
        pre->next = new_node;
        new_node->next = cur;
    }
}

bool node__safe__compare(NodeAddress n1, NodeAddress n2, LinkedListComparisonFn comp_fn) {
    if(n1 == NULL) {
        return false;
    }else if (n2 == NULL) {
        return true;
    }
    return comp_fn(n1->info, n2->info);
}

NodeIterator node_iter_init(NodeAddress head) {
    NodeIterator iter;
    iter.cur = head;
    return iter;
}

NodeAddress node_iter_next(NodeIterator *iter) {
    NodeAddress info;
    info = iter->cur;
    if(info == NULL) {
        return info;
    }
    iter->cur = iter->cur->next;
    return info;
}

bool node_iter_is_end(NodeIterator iter) {
    if(iter.cur != NULL) {
        return true;
    }
    return false;
}

NodeInfoType node_at(NodeAddress head, int index ) {
    NodeAddress cur = head;
    int i = 0;
    while(cur != NULL && i != index) {
        i++;
        cur = cur->next;
    }

    if(cur == NULL) {
        assert("Out of bound :angr:" && false);
    }

    return cur->info;
}

#endif // LINKED_LIST_IMPLEMENTATION

#endif // LINKED_LIST_H