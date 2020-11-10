#include<stdio.h>
#include <stdlib.h>
#include"Headers\\WriteToFile.h"

FILE *fwptr;

boolean fileWRightSetup(char *file_path, char *file_name, char *file_type){
	/*This function set the path, name and type of the file we would like to write.*/
	char *ptr = strMerge(5, file_path, "\\", file_name, ".",  file_type);
	if(*file_path == 0){
		free(ptr);
		ptr = strMerge(3, file_name, ".",  file_type);
	}

	fwptr = fopen(ptr, "w");
	free(ptr);

	if (fwptr == NULL){
		fprintf(stderr, "Cant write values into file name <%s.%s> on path: <%s> (If path is empty, this is the local folder)\n", file_name, file_type, file_path);
		return False;
	}
	return True;
}

void writeTwoListsAsColumn(List *list1, List *list2){
	/*This function write two list as two column. each cell of the two list will be a raw*/
	int size = sizeOfList(list1), index = 0;
	if(size >  sizeOfList(list2))
		size = sizeOfList(list2);

	/*Wite all data*/
	while (size > index)
	{
		fprintf(fwptr,"%s\t%s\n",getItemFromList(list1, index), getItemFromList(list2, index));
		index++;
	}
}

void writeListsAsColumn(List *list){
	/*This function write two list as two column. each cell of the two list will be a raw*/
	int size = sizeOfList(list), index = 0;

	/*Wite all data*/
	while (size > index)
	{
		fprintf(fwptr,"%s\n",getItemFromList(list, index));
		index++;
	}
}

void writeTwoNumAsRaw(int num1, int num2){
	/*This function write two list as two column. each cell of the two list will be a raw*/

	fprintf(fwptr,"%d\t\t%d\n",num1, num2);
}

void stopWriting(){
	fclose(fwptr);
}