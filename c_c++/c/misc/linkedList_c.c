#include"vector_c.c"
#include"linkedList_c.h"

#define __DEBUG_LINKED_LIST_C__

void* insertLinkedVoid(list_l* list, size_t dataSize, const void* value)
{
	if(list->vec.size != 0)
	{
		*(size_t*)((intptr_t)(list->vec.buffer) + list->last) =
			(size_t)list->vec.size;
		list->last = (size_t)list->vec.size;
	}

    size_t index = ~0;
    if(insertVoid(&(list->vec), sizeof(size_t), &index) == NULL) return NULL;;
    if(insertVoid(&(list->vec), dataSize, value) == NULL) return NULL;;
    list->vec.size = ((list->vec.size-1) & ~0x7)+8;
}

int getLastLinkedVoid(list_l* list, size_t dataSize, void* value)
{
	if(list == NULL || value == NULL) return 1;
	if(list->vec.size == 0) return 1;

	intptr_t index = index = list->first;
	if(list->first != list->last)
	{
		for(; *(size_t*)(((intptr_t)list->vec.buffer)+index) != (list->last);
			index = *(size_t*)(((int8_t*)(list->vec.buffer))[index]));
		*(size_t*)(((intptr_t)list->vec.buffer)+index) = ~0;
	}
	memcpy(value, ((intptr_t)list->vec.buffer)+list->last+sizeof(size_t),
			dataSize);

	*(size_t*)(((intptr_t)list->vec.buffer)+list->last) =
		*(size_t*)(((intptr_t)list->vec.buffer) + list->vec.size
					- sizeof(size_t) - dataSize);
	list->vec.size-=((sizeof(size_t)+dataSize-1) & ~0x7)+8;

	return 0;
}

int getLinkedVoidAt(list_l* list, size_t dataSize, size_t index, void* value)
{
	if(list == NULL || value == NULL) return 1;
	if(list->vec.size == 0
		|| list->vec.size < index*(((sizeof(size_t)+dataSize-1) & ~0x7)+8)
			+dataSize)
		return 1;

	size_t cur = list->first;
	for(size_t curIndex = 0;
		curIndex != index;
		cur = *(size_t*)((intptr_t)(list->vec.buffer) + cur),
		 ++curIndex);
	memcpy(value, ((intptr_t)list->vec.buffer) + cur+sizeof(size_t), dataSize);

	return 0;
}

int setLinkedVoidAt(list_l* list, size_t dataSize, size_t index, void* value)
{
	if(list == NULL || value == NULL) return 1;
	if(list->vec.size < index*(((sizeof(size_t)+dataSize-1) & ~0x7)+8)
			+dataSize)
		return 1;

	size_t cur = list->first;
	for(size_t curIndex = 0;
		curIndex != index;
		cur = *(size_t*)((intptr_t)(list->vec.buffer) + cur),
		 ++curIndex);
	memcpy((intptr_t)list->vec.buffer + cur+sizeof(size_t), value, dataSize);

	return 0;
}

void* insertLinkedVoidAt(list_l* list, size_t dataSize, size_t index,
						const void* value)
{
	if(list == NULL || value == NULL) return 1;
	if(list->vec.size+dataSize+sizeof(size_t) < (index)*(((sizeof(size_t)+dataSize-1) & ~0x7)+8)
			+dataSize)
		return 1;

	size_t next;
	if(index != 0)
	{
		size_t cur = list->first;
		for(size_t curIndex = 0;
			curIndex != index-1;
			cur = *(size_t*)((intptr_t)(list->vec.buffer) + cur),
			 ++curIndex);

		next = cur != list->last ?
			*(size_t*)((intptr_t)(list->vec.buffer) + cur) : ~0;
		*(size_t*)((intptr_t)(list->vec.buffer) + cur) = list->vec.size;
		list->last = cur != list->last ? list->last : list->vec.size;
	}
	else
	{
		next = list->first;
		list->first = list->vec.size;
	}

	if(insertVoid(&(list->vec), sizeof(size_t), &next) == NULL) return NULL;
    if(insertVoid(&(list->vec), dataSize, value) == NULL) return NULL;

    list->vec.size = ((list->vec.size-1) & ~0x7)+8;
}

void* reserveLinkedVoid(list_l* list, size_t dataSize, size_t count)
{
	return reserveVoid(&list->vec, dataSize+sizeof(size_t), count);
}

void destroyLinkedList(list_l* list)
{
	list->first = 0;
	list->last = 0;
	destroyVector(&list->vec);
}

#ifdef __DEBUG_LINKED_LIST_C__
#include<stdio.h>

typedef struct {
    size_t next;
    int data;
} data_list;

void printfList(const char* msg, list_l* list)
{
    puts(msg);
    printf("first: %d\nlast: %d\nbuffer: %p\nsize: %d\nreserve: %d\nvalues:\n",
		list->first, list->last, list->vec.buffer, list->vec.size, list->vec.reserve);
	if(list->vec.size == 0)
	{
		puts("finished...\n");
		return;
	}

	data_list* data;
    for(data = (data_list*)((intptr_t)list->vec.buffer + list->first);
		data->next != ~0;
		data = (data_list*)((intptr_t)list->vec.buffer + data->next))
    {
        printf("\t%d\t%d\n", data->data, data->next);
    }
    printf("\t%d\t%d\n", data->data, data->next);
    puts("finished...\n");
}

int main()
{
	list_l list;
    initVoidList(&list);
    printfList("creating vector...", &list);

    for(size_t k = 0; k < 2; ++k)
    {
        int value = 58;
        insertLinkedVoid(&list, sizeof(int), &value);
        printfList("inserting value...", &list);

        /*int valueArr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        insertVoid(&vec, sizeof(valueArr), &valueArr);
        printfVector("inserting array...", vec);*/

        getLastLinkedVoid(&list, sizeof(int), &value);
        printf("getting last value...\nlast value: %d\n", value);
        printfList(NULL, &list);

        /*getLastVoid(&list, sizeof(valueArr), &valueArr);
        printf("getting last array...\nlast array: \n");
        for(size_t i = 0; i < sizeof(valueArr)/sizeof(int); ++i)
        {
            printf("\t[%d]: %d\n", i, valueArr[i]);
        }
        printfVector(NULL, list);*/

        /*insertVoid(&list, sizeof(valueArr), &valueArr);
        printfVector("inserting array...", list);*/

        value = 59;
        insertLinkedVoid(&list, sizeof(int), &value);
        printfList("inserting value...", &list);
        value = 60;
        insertLinkedVoid(&list, sizeof(int), &value);
        printfList("inserting value...", &list);
        value = 61;
        insertLinkedVoid(&list, sizeof(int), &value);
        printfList("inserting value...", &list);
        value = 62;
        insertLinkedVoid(&list, sizeof(int), &value);
        printfList("inserting value...", &list);
        value = 63;
        insertLinkedVoid(&list, sizeof(int), &value);
        printfList("inserting value...", &list);

        value = 1;
        insertLinkedVoidAt(&list, sizeof(int), 0, &value);
        printfList("inserting value at...", &list);
        value = 0;
        insertLinkedVoidAt(&list, sizeof(int), 0, &value);
        printfList("inserting value at...", &list);
        value = 2;
        insertLinkedVoidAt(&list, sizeof(int), 2, &value);
        printfList("inserting value at...", &list);
        value = 5;
        insertLinkedVoidAt(&list, sizeof(int), 5, &value);
        printfList("inserting value at...", &list);
        value = 8;
        insertLinkedVoidAt(&list, sizeof(int), 8, &value);
        printfList("inserting value at...", &list);
        value = 10;
        insertLinkedVoidAt(&list, sizeof(int), 10, &value);
        printfList("inserting value at...", &list);

        getLinkedVoidAt(&list, sizeof(int), 3, &value);
        printf("getting 3rd value...\nlast value: %d\n", value);
        printfList(NULL, &list);

        value = 25;
        setLinkedVoidAt(&list, sizeof(int), 3, &value);
        printfList("setting 3rd value...", &list);

        reserveLinkedVoid(&list, sizeof(int), 100);
        printfList("reserving 100 ints...", &list);

        reserveLinkedVoid(&list, sizeof(int), 200);
        printfList("reserving 200 ints...", &list);

        destroyLinkedList(&list);
        printfList("deleting vector...", &list);
        printf("------------------------------------------------------------------\n\n");

        /*if(k == 0)
        {
            initVector(&list, 5, int, 0, 1, 2, 3, 4);
            printfVector("creating vector...", list);
        }*/
    }

    /*initVector(&list, 5, vector_v, 0, 1, 2, 3, 4);
    printfVector("creating vector...", list);

    destroyVector(&list);
    printfVector("deleting vector...", list);*/

    return 0;
}
#endif
