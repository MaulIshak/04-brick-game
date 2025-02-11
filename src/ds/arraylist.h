#include <stdint.h>

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

typedef struct ArrayList
{
    int capacity;
    int len;
    void* items;
};

#define ArrayList_Init(T, capacity) ArrayList_Init_(capacity, sizeof(T))

ArrayList ArrayList_Init_(int capacity, int size);
void* Arraylist_Get(ArrayList *arr, int index);
void* Arraylist_Push(ArrayList *arr, void* item);


#endif //ARRAY_LIST_H