#include<stdint.h>
#include<string.h>

#include"vector_c.h"

typedef struct {
    size_t first;
    size_t last;
    vector_v vec;
} list_l;

#define initVoidList(pList) { \
((list_l*)(pList))->vec.reserve = 0; \
((list_l*)(pList))->vec.size = 0; \
((list_l*)(pList))->vec.buffer = NULL; \
((list_l*)(pList))->first = 0; \
((list_l*)(pList))->last = 0;\
}

void* insertLinkedVoid(list_l* list, size_t dataSize, const void* value);
int getLastLinkedVoid(list_l* list, size_t dataSize, void* value);
int getLinkedVoidAt(list_l* list, size_t dataSize, size_t index, void* value);
int setLinkedVoidAt(list_l* list, size_t dataSize, size_t index, void* value);
void* insertLinkedVoidAt(list_l* list, size_t dataSize, size_t index,
						const void* value);

void* reserveLinkedVoid(list_l* list, size_t dataSize, size_t count);
void destroyLinkedList(list_l* list);
