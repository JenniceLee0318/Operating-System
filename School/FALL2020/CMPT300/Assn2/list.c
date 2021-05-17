#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"


static List lists[LIST_MAX_NUM_HEADS];
static int listCount = 0;

Node nodes[LIST_MAX_NUM_NODES];
int nodeCount = 0;
void printNodes(){
    printf("List of nodes: ");
    for(int i = 0; i < nodeCount; i++) {
        printf("%d ",*(int *) nodes[i].element);
    }
    printf("\n");
}
void List_info(List* pList) {
    //Print out list
    //Print out currIdx
    //Print out curr
    printf("-----------------------\n");

    if(pList->head != NULL){
         Node *c = pList->head;
        printf("List values forward: ");
        while(c->next != NULL) {
            printf("%d ",*(int *) c->element);   
            c = c->next;
        }
        printf("%d\n",*(int *) c->element);
    }
   
    // if(pList->tail != NULL){
    //     Node *d = pList->tail;
    //     printf("List values backwards: ");
    //     while(d->prev != NULL) {
    //         printf("%d ",*(int *) d->element);   
    //         d = d->prev;
    //     }
    //     printf("%d\n",*(int *) d->element);
    // }
    

    if(pList->curr){
        printf("curr: %d | ",*(int *) pList->curr->element);
    }
    printf("currIdx: %d\n",pList->currIdx);
    printf("size: %d\n",pList->size);
    printf("-----------------------\n");
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.

List* List_create(){
    if(listCount >= LIST_MAX_NUM_HEADS){
        printf("Too many heads.");
        return NULL;
    }

    
    lists[listCount].size = 0;
    lists[listCount].listIdx = listCount;
    lists[listCount].currIdx = -1;
    listCount ++;
    return &lists[listCount - 1];
}

// Returns the number of items in pList.
int List_count(List* pList){
    return pList->size;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    if(List_count(pList) == 0){
        pList->curr = NULL;
        return NULL;
    }
    pList->curr = pList->head;
    pList->currIdx = 0;
    return pList->head->element;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    if(List_count(pList) == 0){
        pList->curr = NULL;
        return NULL;
    }
    pList->curr = pList->tail;
    pList->currIdx = pList->size - 1;
    return pList->tail->element;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    if(pList->size <= pList->currIdx) { //Beyond
        return NULL;
    }
    pList->currIdx ++;
    pList->curr = pList->curr->next;
    return pList->curr->element;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
     if(0 >= pList->currIdx) { //Before
        return NULL;
    }
    pList->currIdx --;
    pList->curr = pList->curr->prev;
    return pList->curr->element;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    if(pList->curr == NULL) return NULL;
    return pList->curr->element;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem){
    if(nodeCount >= LIST_MAX_NUM_NODES){
        printf("Too many nodes");
        return -1;
    }
    if(0 > pList->currIdx) {
        //Add to head, append previous head to next
        nodes[nodeCount].nodeIdx = nodeCount;
        nodes[nodeCount].next = pList->head;
        nodes[nodeCount].prev = NULL;
        nodes[nodeCount].element = pItem;
        if(pList->size != 0){
            pList->head->prev = &nodes[nodeCount];
        }
        pList->head = &nodes[nodeCount];
        pList->size ++;
        pList->curr = &nodes[nodeCount];
        pList->currIdx = 0;

    }else if(pList->currIdx >= pList->size) {
        //Add to tail
        nodes[nodeCount].nodeIdx = nodeCount;
        nodes[nodeCount].next = NULL;
        nodes[nodeCount].prev = pList->tail;
        nodes[nodeCount].element = pItem;

        pList->tail->next = &nodes[nodeCount];
        pList->tail = &nodes[nodeCount];
        pList->curr = &nodes[nodeCount];
        pList->size ++;
        pList->currIdx = pList->size - 1;
    }else{
        //Otherwise add right after the current item
        nodes[nodeCount].nodeIdx = nodeCount;
        nodes[nodeCount].next = pList->curr->next;
        nodes[nodeCount].prev = pList->curr;
        nodes[nodeCount].element = pItem;
        // int nodeCur = pList->curr->nodeIdx;
        // pList->curr = &nodes[nodeCount];

        if(pList->curr->next == NULL) { //Adding to the end
            pList->tail = &nodes[nodeCount];
        }else{
            pList->curr->next->prev = &nodes[nodeCount];
        }
        
        pList->curr->next = &nodes[nodeCount];
        pList->curr = &nodes[nodeCount];
        pList->size ++;
        pList->currIdx ++;
    }
    
    nodeCount ++;
    return 0;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem){
    if(nodeCount >= LIST_MAX_NUM_NODES){
        printf("Too many nodes");
        return -1;
    }
    if(0 > pList->currIdx) {
        //Add to head, append previous head to next
        nodes[nodeCount].nodeIdx = nodeCount;
        nodes[nodeCount].next = pList->head;
        nodes[nodeCount].prev = NULL;
        nodes[nodeCount].element = pItem;
        if(pList->size != 0) {
            pList->head->prev = &nodes[nodeCount];
        }else{
            pList->tail = &nodes[nodeCount];
        }
        pList->head = &nodes[nodeCount];
        pList->curr = &nodes[nodeCount];
        pList->size ++;
        pList->currIdx = 0;

    }else if(pList->currIdx >= pList->size) {
        //Add to tail
        nodes[nodeCount].nodeIdx = nodeCount;
        nodes[nodeCount].next = NULL;
        nodes[nodeCount].prev = pList->tail;
        nodes[nodeCount].element = pItem;

        pList->tail->next = &nodes[nodeCount];
        pList->tail = &nodes[nodeCount];
        pList->curr = &nodes[nodeCount];
        pList->size ++;
        pList->currIdx = pList->size - 1;
    }else{
        //Otherwise add right before the current item
        nodes[nodeCount].nodeIdx = nodeCount;
        nodes[nodeCount].next = pList->curr;
        nodes[nodeCount].prev = pList->curr->prev;
        nodes[nodeCount].element = pItem;

        // if(pList->curr->head )
        if(pList->curr->prev == NULL) { //Adding to the front
            pList->head = &nodes[nodeCount];
        }else{
            pList->curr->prev->next = &nodes[nodeCount];
        }
        
        pList->curr->prev = &nodes[nodeCount];
        pList->curr = &nodes[nodeCount];
        pList->size ++;
    }
    nodeCount ++;
    return 0;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    if(nodeCount >= LIST_MAX_NUM_NODES){
        printf("Too many nodes");
        return -1;
    }
    nodes[nodeCount].nodeIdx = nodeCount;
    nodes[nodeCount].next = NULL;
    nodes[nodeCount].prev = pList->tail;
    nodes[nodeCount].element = pItem;

    pList->curr = &nodes[nodeCount];
    if(pList->size == 0) {
        pList->head = &nodes[nodeCount];
    }else{
        pList->tail->next = &nodes[nodeCount];
    }
    pList->tail = &nodes[nodeCount];
    pList->size ++;
    pList->currIdx = pList->size - 1;
    
    nodeCount ++;
    return 0;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
     if(nodeCount >= LIST_MAX_NUM_NODES){
        printf("Too many nodes");
        return -1;
    }
    nodes[nodeCount].nodeIdx = nodeCount;
    nodes[nodeCount].next = pList->head;
    nodes[nodeCount].prev = NULL;
    nodes[nodeCount].element = pItem;

    if(pList->size == 0) {
        pList->tail = &nodes[nodeCount];
    }
    else if(pList->size != 0){
        pList->head->prev = &nodes[nodeCount];
    }
    pList->head = &nodes[nodeCount];
    pList->curr = &nodes[nodeCount];
    pList->size ++;
    pList->currIdx = 0;
    
    nodeCount ++;
    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    if(0 > pList->currIdx || pList->currIdx >= pList->size) {
        return NULL;
    }

    void* item = pList->curr->element;
    int theNodeIdx = pList->curr->nodeIdx;
    if(pList->currIdx == 0) { //First item
        if(pList->curr->next != NULL) { //There are more elements
            pList->curr = pList->curr->next;
            pList->curr->prev = NULL;
            pList->head = pList->curr;

            pList->currIdx = 0;
        }else{ //no next; only element
            pList->head = NULL;
            pList->tail = NULL;
            pList->currIdx = -1;
        }
    }else if(pList->currIdx == pList->size -1){ //Last item
        pList->tail = pList->curr->prev;
        pList->tail->next = NULL;
        pList->curr = NULL;
    }else{
        pList->curr->prev->next = pList->curr->next;
        pList->curr->next->prev = pList->curr->prev;
        
        pList->curr = pList->curr->next;
    }
    //swap empty node space with last node space 
    nodes[theNodeIdx] = nodes[nodeCount-1];
    nodeCount --;
    pList->size --;
    return item;
}


// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){
    if(pList2->size == 0) return;
    
    //Add head of list2 to tail of list1
    if(pList1->head != NULL) { //list1 has elements;
        if(pList2->size != 0){ //if list2 has elements
            pList1->tail->next = pList2->head;
            pList1->tail->next->prev = pList1->tail;
            pList1->tail = pList2->tail;
            pList1->size += pList2->size;
        }
        //if list2 is empty, do nothing;
    }else{ //list1 is empty
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->size = pList2->size;
    }

    pList2->size = 0;
    pList2->head = NULL;
    pList2->tail = NULL;
    pList2->curr = NULL;
    pList2->currIdx = -1;
    
    int theListIdx = pList2->listIdx;
    lists[theListIdx] = lists[listCount-1];
    listCount --;

    pList2->listIdx = -1;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
    if(pItemFreeFn == NULL) return;

    List_first(pList);
    for(int i = 0; i < pList->size; i++) {
        pItemFreeFn(pList->curr->element);
        
        int theNodeIdx = pList->curr->nodeIdx;
        nodes[theNodeIdx] = nodes[nodeCount-1];
        nodeCount --;
        
        List_remove(pList);
    }

    pList->size = 0;
    pList->head = NULL;
    pList->tail = NULL;
    pList->curr = NULL;
    pList->currIdx = -1;
    
    int theListIdx = pList->listIdx;
    lists[theListIdx] = lists[listCount-1];
    listCount --;

    pList->listIdx = -1;

}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    if(pList->size == 0) return NULL;

    List_last(pList); //set to last
    void *lastItem = pList->curr->element;
    int theNodeIdx = pList->curr->nodeIdx;
    List_remove(pList);

    List_last(pList); //set to last again

    nodes[theNodeIdx] = nodes[nodeCount-1];
    nodeCount --;
    
    return lastItem;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){ //pComparisonArg is the value
    if(pList->size == 0 ||pComparator == NULL || pComparisonArg == NULL) return NULL;

    bool found = pComparator(pList->curr->element, pComparisonArg);

    while(!found && pList->curr->next != NULL) {
        List_next(pList);
        found = pComparator(pList->curr->element, pComparisonArg);
    }
    if(found){
        return pList->curr->element;
    }else{
        pList->curr = NULL;
        pList->currIdx = pList->size;
    }

    return NULL;
}
