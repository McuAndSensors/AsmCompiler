#include<stdio.h>
#include <stdlib.h>
#include"Headers\\StringAdvanceFunc.h"


boolean startWith(char *source, char *start_with_str){
	/*This function check if a string is start the same as the other string.*/
	int index = 0;
	while (source[index] && start_with_str[index])
	{
		if(source[index] - start_with_str[index])
			return False;
		index++;
	}
	if(start_with_str[index]) /*If the string of "start_with_str" bigger then source then it can be true.*/
		return False;
	return True;
}

void clearWhiteSpaceFromHead(char *source){
	/*This function delete all sapces from the head of the string*/
	int index = 0;
	char *temp;

	if(source == NULL)
		return;

	if(*source == 0)
		return;

	temp = allocatCpyStr(source);

	while (source[index] == ' ' || source[index] == '	'){index++;}
	strcpy(source, temp + index);
	free(temp);
}

void clearWhiteSpaceFromTail(char *source){
	/*This function delete all sapces from the tail of the string*/
	int index = strlen(source) - 1;

	if(index == 0)
		return;

	while (index){
		if(source[index] != ' ' && source[index] != '	')
			break;
		index--;
	}
	source[index + 1] = 0;
}

void replaceAll(char *source, char ch1, char to_ch2){
	/*Rplace any character that match to "ch1" in the string to the character in var "to_ch2"*/
	int index = 0;
	while (source[index]){
		if(source[index] == ch1)
			source[index] = to_ch2;
		index++;
	}
}

void replaceFirst(char *source, char ch1, char to_ch2){
	/*This function replace only the closest character from the head to the desiered char*/
	int index = 0;

	if(source == NULL)
		return;

	while (source[index] != ch1 && source[index] != 0){index++;}
	source[index] = source[index] ? to_ch2 : 0;
}

int getNextWhiteSpaceIndex(char *source){
	/*This function return the index of the closest white space*/
	int index = 0;

	while (source[index] != ' ' && source[index] != '	' && source[index] != 0){index++;}
	return index;
}

int getNextWordWhiteSpaceIndex(char *source){
	/*This function return the index of the closest white space after the first word that is can find*/
	/*If the following ford end the string, it will return the end of the string index*/
	int index = 0;

	while (source[index] == ' ' || source[index] == '	'){index++;}
	while (source[index] != ' ' && source[index] != '	' && source[index] != 0){index++;}
	return index;
}

void clearFirstWord(char *source){
	/*This function remove the first word from the string*/
	int index = 0;

	while (source[index] == ' ' || source[index] == '	'){index++;}
	while (source[index] != ' ' && source[index] != '\t' && source[index] != 0){index++;}

	while (*(source+index - 1))
	{
		*source = *(source + index);
		source++;
	}
}

char *getFirstWord(char source[]){
	/*This function return the fist word on the string.*/
	/*Example1: <"  asdf sdf sdfff>; return "   asdf" ; Example2: <"abc-dc aa> return: "abc-da"*/
	char *ptr = NULL;
	int index = 0;

	if(source == NULL)
		return ptr;

	while (source[index] == ' ' || source[index] == '\t'){index++;}
	while (source[index] != ' ' && source[index] != '\t' && source[index] != 0){index++;}

	ptr = (char*)malloc((index + 1) * sizeof(char));
	
	if(ptr == NULL){
		fprintf(stderr, "System is out of memory!");
		exit(0);
	}

	strncpy(ptr, source, index);
	*(ptr + index) = 0;
	return ptr;
}

char **split(char *str, char ch){
	/*This function get a string with a charcter to split the string into list*/
	/*this function will return a splitted list of string according to the char location*/
	char **list, *word_ptr;
	int rows = 2, index_str = 0, index_split = 0;

	/*Count the number of rows requiered*/
	while (str[index_str])
	{
		if(str[index_str++] == ',')
			rows++;
	}

	list = (char**)malloc(rows * sizeof(char*));
	if(list == NULL){
		fprintf(stderr, "System is out of memory!");
		exit(0);
	}

	/*fill each row with the relevant data*/
	index_str = 0;
	word_ptr = str;
	while (*word_ptr)
	{
		if(word_ptr[index_str] == ch || word_ptr[index_str] == 0){
			list[index_split] = (char*)malloc((index_str+1) * sizeof(char));

			if(list[index_split] == NULL){
				fprintf(stderr, "System is out of memory!");
				exit(0);
			}

			strncpy(list[index_split], word_ptr, index_str);
			*(list[index_split] + index_str) = 0;
			word_ptr += word_ptr[index_str] ? index_str + 1 : index_str;
			index_str = 0;
			index_split++;
		}
		index_str++;
	}
	list[index_split] = (char*)malloc(sizeof(char));

	if(list[index_split] == NULL){
		fprintf(stderr, "System is out of memory!");
		exit(0);
	}
	strcpy(list[index_split], "");
	return list;
}

boolean isValidNum(char *str){
	/*This function check if the string is integer number*/
	int index = 0;
	if (*str == '-' || *str == '+'){
		index++;
	}
	
	while((str[index] >= '0' && str[index] <= '9') && str[index] != 0){
		index++;
	}
	if(str[index] == 0)
		return True;
	return False;
}

int convertStrToInt(char *str){
	/*This function check if the string is integer number*/
	int index = 0, res = 0, num_type = 1;
	if (*str == '-' || *str == '+'){
		index++;
		if(*str == '-')
			num_type *= -1;
	}
	
	while((str[index] >= '0' && str[index] <= '9') && str[index] != 0){
		res *= 10;
		res += str[index] - '0';
		index++;
	}
	return res * num_type;
}

int getSplitDataSize(char **list){
	/*Return the size of arraly list of string in this spical structure when list of strings ends with "" */
	int index = 0;
	while (*list[index])
	{
		index++;
	}
	return index;
}

char *strMerge(int strings_to_merge, ...){
	/*This function is merging a multiple strings into one big string*/
	/*This function is allocating new string in the size of all the strings that was given and return the pointer of the new merged string.*/
	char *merge, *str_ptr; 
	va_list var_ptr;
	int index, index_ptr = 0, strings_amounts = strings_to_merge, str_size = 0;

	/*Calculate the size of the string requier to allocate*/
	va_start(var_ptr, strings_to_merge);
	while (strings_amounts--)
	{
		str_ptr = va_arg(var_ptr, char*);
		str_size += strlen(str_ptr);
	}

	/*Allocatte the string that will contain all strings provieded*/
	merge = (char*)malloc(str_size * sizeof(char*));
	if (merge == NULL)
		exit(0);

	/*Copy all strings into the new allocated big string.*/
	strings_amounts = strings_to_merge;
	va_start(var_ptr, strings_to_merge);
	while (strings_amounts--)
	{
		index = 0;
		str_ptr = va_arg(var_ptr, char*);
		while (*str_ptr)
		{
			merge[index_ptr] = *str_ptr++;
			index++;
			index_ptr++;
		}
	}

	/*Lock the string*/
	merge[index_ptr] = 0;
	return merge;
}

boolean stringComp(char *str1, char *str2){
	int index = 0;

	while (str1[index] && str2[index])
	{
		if((str1[index] - str2[index]) != 0)
			return False;
		index++;
	}
	return True;
}

char *getFirstWordClean(char *str){
	/*This function getting the first word of the string and clean it from white spaces*/
	char *temp, *mem, *res;
	temp = getFirstWord(str);
	mem = temp;

	clearWhiteSpaceFromHead(temp);
	clearWhiteSpaceFromTail(temp);

	res = allocatCpyStr(temp);
	free(mem);

	return res;
}

char *allocatCpyStr(char *str){
	/*This function duplicate a string and return the pointer of the duplicated one.*/
	int size = strlen(str) + 1;
	char *temp = 0;
	
	temp = (char*)malloc(size * sizeof(char));
	if(temp == NULL){
		fprintf(stderr, "System is out of memory!");
		exit(0);
	}
	strcpy(temp, str);
	return temp;
}

char *convertIntToStr(int value){
	int reversed_value = 0, num_char_size = 0, index = 0;
	char *int_str;
	boolean flag = False;

	if(value == 0)
		return allocatCpyStr("0");

	if(0 > value){
		value *= -1;
		flag = True;
		num_char_size++;
	}
	reversed_value = value;

	/*count the digit amount*/
	while (value)
	{
		value = value / 10;
		num_char_size++;
	}

	/*Allocate a string*/
	num_char_size++;
	int_str = (char*)malloc(num_char_size * sizeof(char));
	if(int_str == NULL){
		fprintf(stderr, "System is out of memory!");
		exit(0);
	}

	/*copy the int value into the string*/
	while (reversed_value)
	{
		int_str[index++] = reversed_value % 10 + '0';
		reversed_value /= 10;
	}

	if(flag)
		int_str[index++] = '-';

	int_str[index] = 0;

	reverseString(int_str);
	return int_str;
}

char *convertIntTo24BitHexStr(int value){
	int index = 0;
	char hex_str[9] = "000000x0";

	/*In case of negative value, use 2'compement*/
	if(0 > value){
		value != value;
		value &= 0xffffff;
	}

	/*copy the int value into the string*/
	while (value && index < 6)
	{
		if(value % 16 > 9)
			hex_str[index++] = value % 16 + 'A' - 10;
		else
			hex_str[index++] = value % 16 + '0';
		value /= 16;
	}
	reverseString(hex_str);
	return allocatCpyStr(hex_str);
}

void reverseString(char *str){
	/*This function reverse a given string*/
	int length, c;
	char *begin, *end, temp;
 
	length = strlen(str);
	begin = str;
	end = str;
 
	for (c = 0; c < length - 1; c++)
		end++;
 
	for (c = 0; c < length/2; c++)
	{        
		temp = *end;
		*end = *begin;
		*begin = temp;
 
		begin++;
		end--;
	}
}