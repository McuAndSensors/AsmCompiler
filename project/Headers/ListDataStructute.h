#include"Headers\\PrjTypes.h"

List *createList();
void appendToList(List *list, char *str, boolean allow_duplication);
void removeItemByValueFromList(List *list, char *item);
void removeItemByIndexFromList(List *list, int index);
void printList(List *list);
char *getItemFromList(List *list, int index);
int sizeOfList(List *list);
void clearList(List *list);
boolean isItemInList(List *list, char *item);
int getIndexByValueFromList(List *list, char *item);
void appendListToList(List *dest, List *source);
