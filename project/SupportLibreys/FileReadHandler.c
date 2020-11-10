#include<stdio.h>
#include <stdlib.h>
#include"Headers\\FileReadHandler.h"

#define MAX_LINE_SIZE 1000  /*This is the limit for max line size*/

FILE *fptr;
char c, *line_str = NULL, *temp = NULL;
int line_size = 0;

static char *allocateStr(int str_size){
	/*privte function for this file only to allocate a string*/
	char *ptr = (char*) malloc(str_size*sizeof(char));
	temp = ptr;
	if (ptr == NULL)
		return 0;
	return ptr;
}

boolean openAsmFile(char *file_path){
	fptr = fopen(file_path, "rb");
	
	if (fptr == NULL){
		return False;
		c = EOF;
	}
	c = 0;
	return True;
}

char *getNextLine(){ 
	line_size = 0;

	/*Release string before allocating a new one*/
	if (line_str != NULL)
		free(temp);

	/*If string cant be allocated, return empty string*/
	if(!(line_str = allocateStr(MAX_LINE_SIZE)) || (fptr == NULL))
		return "";

	/*Trunsfer the current line into the new string*/
	while (((c = getc(fptr)) != '\n') && (c != EOF)){
		line_size++;
		*line_str = c;
		line_str++;
	}

	/*Close the string*/
	*line_str = 0;

	return (line_str - line_size); /*Return the string address*/
}

boolean isNextLineValid(){
	if (c == EOF)
		return False;
	return True;
}

void pointToPreLineOnFile(){
	/*This function set the pointer to point to orivious line*/
	fseek(fptr, -1*(line_size + 1), SEEK_CUR);
}

void closeAsmFile(){
	fclose(fptr);
}