#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Headers\\StringAdvanceFunc.h"
#include"Headers\\ListDataStructute.h"

static void matAllocationVar(char **ptr);
static void arrAllocationVar(char *ptr);
static void arrAllocationVar(char *ptr);

List *createList(){
	/*This function craete a new List-Data-Structure*/

	/*Allocate new list*/
	List *new_list = (List*) malloc(sizeof(List));
	(*new_list).size = 0;
	(*new_list).head = NULL;

	/*Allocate new matrix in the size of one*/
	(*new_list).head = (char**)malloc(sizeof(char*));
	matAllocationVar((*new_list).head);

	/*Allocate new string in the size of one*/
	*((*new_list).head) = (char*)malloc(sizeof(char));
	arrAllocationVar(*((*new_list).head));

	/*Set the new string to be empty*/
	*((*new_list).head) = NULL;

	return new_list;
}

void appendToList(List *list, char *str, boolean allow_duplication){
	/*This function add new string into the list*/

	if(!allow_duplication){
		if(isItemInList(list, str))
			return;
	}
	/*Increase matrix size by one*/
	(*list).head = (char**)realloc((*list).head, (sizeOfList(list) + 1) * sizeof(char*));
	matAllocationVar((*list).head);

	/*Allocate the string*/
	(*list).head[(*list).size] = (char*)malloc((strlen(str) + 1) * sizeof(char*));
	arrAllocationVar((*list).head[(*list).size]);

	strcpy((*list).head[(*list).size], str);	/*Copy item to the list*/

	(*list).size++;	/*Increase size of List memory*/
}

void removeItemByValueFromList(List *list, char *item){
	/*This function remove the item from the list by its value*/
	int index = getIndexByValueFromList(list, item);

	if(index == -1)
		return;
	removeItemByIndexFromList(list, index);
}

void removeItemByIndexFromList(List *list, int index){
	/*This function remove an item by index location*/
	int size = 0;
	List *temp = createList();

	while (size < (*list).size)
	{
		if(size != index)
			appendToList(temp, getItemFromList(list, size), True);
		size++;
	}
	clearList(list);
	(*list).head = (*temp).head;
	(*list).size = (*temp).size;
}

void printList(List *list){
	/*This function prints all item in the list*/
	int index = 0;

	if((*list).size == 0){
		fprintf(stdout, "[]\n");
		return;
	}

	fprintf(stdout, "[");
	while (((*list).size - 1) > index)
	{
		fprintf(stdout, "%s, ", (*list).head[index++]);
	}
	fprintf(stdout, "%s]\n", (*list).head[index]);
}

char *getItemFromList(List *list, int index){
	static char empty[] = "";
	if(index > (*list).size)
		return empty;
	return (*list).head[index];
}

int sizeOfList(List *list){
	/*This functin return the size of the list*/
	return (*list).size;
}

int getIndexByValueFromList(List *list, char *item){
	/*This function return the index of an item by its name*/
	int index = 0;

	if(item == NULL)
		return -1;

	/*Search if there is List-item that match to the given one"*/
	while (sizeOfList(list) > index)
	{
		if(strcmp((*list).head[index], item) == 0){
			return index++;
		}
		index++;
	}
	return -1;
}

void clearList(List *list){
	/*This function release all memory allocation of the list*/
	if((*list).size == 0)
		return;

	(*list).size--;
	while ((*list).size)
	{
		free((*list).head[(*list).size--]);
	}
	(*list).head = (char**)realloc((*list).head, sizeof(char*));
}

boolean isItemInList(List *list, char *item){
	/*This function check if the string provieded is in the list of strings*/
	int index = 0;
	while (sizeOfList(list) > index)
	{
		if(strcmp((*list).head[index++], item) == 0)
			return True;
	}
	return False;
}

void appendListToList(List *dest, List *source){
	/*This function add a list into an existed list*/
	int index = 0;
	while ((*source).size > index)
	{
		appendToList(dest, (*source).head[index++], True);
	}
}

static void matAllocationVar(char **ptr){
	/*This function check if the allocation was sucessful*/
	if(ptr == NULL){
		fprintf(stderr, "System is out of memory!");
		exit(0);
	}
}

static void arrAllocationVar(char *ptr){
	/*This function check if the allocation was sucessful*/
	if(ptr == NULL){
		fprintf(stderr, "System is out of memory!");
		exit(0);
	}
}
