

#include "SPArrayList.h"
#include "SPMainAux.h"
#include <stdio.h>
#include <stdlib.h>

SPArrayList* spArrayListCreate(int maxSize){
	if(maxSize <= 0)
		return NULL;
	SPArrayList *al = (SPArrayList *)malloc(sizeof(SPArrayList));
	if(al==NULL){
		err = MALLOC;
		return NULL;
	}
	al->elements=(int*)calloc(maxSize,sizeof(int));
	if(al->elements==NULL){
		err = CALLOC;
		free(al);
		return NULL;
	}
	al->actualSize = 0;
	al->maxSize = maxSize;
	return al;
}

SPArrayList* spArrayListCopy(SPArrayList* src){
	if(src==NULL)
		return NULL;
	SPArrayList *alCopy = spArrayListCreate(src->maxSize);
	if(alCopy==NULL)
		return NULL;
	alCopy->actualSize = src->actualSize;
	for(int i=0; i<alCopy->actualSize; i++)
		alCopy->elements[i] = src->elements[i];
	return alCopy;
}

void spArrayListDestroy(SPArrayList* src){
	if(src!=NULL){
		free(src->elements);
		free(src);
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if(src==NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){
	if(src==NULL || index>(src->actualSize) || index<0)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if((src->actualSize) >= (src->maxSize))
		return SP_ARRAY_LIST_FULL;
	for(int i=src->actualSize-1 ; i>=index ; i--)
		src->elements[i+1]=src->elements[i];
	src->elements[index]=elem;
	src->actualSize++;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){
	return spArrayListAddAt(src,elem,0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){
	if(src==NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListAddAt(src,elem,src->actualSize);

}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if(src==NULL || index>(src->actualSize-1) || index<0)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	else if(src->actualSize==0)
		return SP_ARRAY_LIST_EMPTY;
	else{
		for(int i=index; i<src->actualSize-1;i++)
			src->elements[i] = src->elements[i+1];
		src->actualSize --;
		return SP_ARRAY_LIST_SUCCESS;
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src,0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if(src==NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(src->actualSize==0)
		return SP_ARRAY_LIST_EMPTY;
	return spArrayListRemoveAt(src,src->actualSize-1);
}

int spArrayListGetAt(SPArrayList* src, int index){
	if(src==NULL || index <0 || index>=src->actualSize)
		return -1;
	return src->elements[index];
}

int spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt(src,0);
}

int spArrayListGetLast(SPArrayList* src){
	if(src==NULL)
		return -1;
	return spArrayListGetAt(src,src->actualSize-1);
}

int spArrayListMaxCapacity(SPArrayList* src){
	if(src==NULL)
		return -1;
	return src->maxSize;
}

int spArrayListSize(SPArrayList* src){
	if(src==NULL)
		return -1;
	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src){
	if(src==NULL || src->actualSize != src->maxSize)
		return false;
	return true;
}

bool spArrayListIsEmpty(SPArrayList* src){
	if(src==NULL || src->actualSize != 0)
		return false;
	return true;
}






