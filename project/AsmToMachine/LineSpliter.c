#include<stdio.h>
#include<stdlib.h>
#include"Headers\\LineSpliter.h"

List *valid_line_struct;
boolean cmd_or_var_flag = True;  /*False: variable decleration line;  True: all other types*/

static boolean isLineStatsWithCmd(char *line);
static boolean isLineStartsWithLabelDec(char *line);
static boolean isLineStartsWithVarDec(char *line);
static char *getNextValidAsmInstructLine();
static List *getFirstLineStartWithCmd(char *line, boolean set_first_index);
static List *getFirstLineStartWithLableDec(char *line);
static List *getFirstLineStartWithVarDec(char *line);

/*Public functions*/
void setCompieldFilePath(char *file_path){
	openAsmFile(file_path);
	valid_line_struct = createList();
}

List *getNextSplitedCmdLine(){
	/*####	This function returns a "List" with a valid command structure	####*/
	/*The struct for any type of command (except variable deleration) is ["LableName", "CommandName", "FirstOperand", "SecondOperand"]*/
	/*Whatever type of operand that dont exist, value will be empty string*/
	/*The struct for variable decleration is ["VariableName", "DataType", "FistCellValue", "SecondCellValue", ...]*/

	char *line = getNextValidAsmInstructLine();

	/*clear the list for the new instruction*/
	clearList(valid_line_struct);

	/*line will be equal to NULL when we reached to the end of the file.*/
	if(line == NULL)
		return valid_line_struct;					

	if(isLineStatsWithCmd(line))
		return getFirstLineStartWithCmd(line, True);

	if(isLineStartsWithVarDec(line))
		return getFirstLineStartWithVarDec(line);

	if(isLineStartsWithLabelDec(line))
		return getFirstLineStartWithLableDec(line);

	return valid_line_struct;
}

boolean SplitedLineIsNotVarDec(){
	return cmd_or_var_flag;
}

/*Private functions*/
static boolean isLineStatsWithCmd(char *line){
	/*Thhis function check if the line satrts with command*/
	char *temp =  getFirstWordClean(line);
	int size = strlen(temp);
	if(*(temp + size - 1) == ':'){
		free(temp);
		return False;
	}
	free(temp);
	return True;
}

static boolean isLineStartsWithLabelDec(char *line){
	/*This function check if the line is Label decleration*/
	char *temp =  getFirstWord(line), *mem;
	int index = 0;

	/*If name ends up with <:> and after it there is no "variable type", then it is lable decleration line.*/
	int size = strlen(temp);
	if(*(temp + size - 1) == ':'){
		free(temp);
		index = getNextWordWhiteSpaceIndex(line);
		temp =  getFirstWord(line + index);
		mem = temp;
		clearWhiteSpaceFromHead(temp);
		clearWhiteSpaceFromTail(temp);
		if(!(startWith(temp, ".data") || startWith(temp, ".string"))){
			free(mem);
			return True;
		}
	}
	free(temp);
	return False;
}

static boolean isLineStartsWithVarDec(char *line){
	/*This function check if the line is Variable decleration*/
	char *temp =  getFirstWord(line), *mem;
	int index = 0, size = strlen(temp);

	/*If name ends up with <:> and after it there is "variable type", then it is variable decleration line.*/
	if(*(temp + size - 1) == ':'){
		free(temp);
		index = getNextWordWhiteSpaceIndex(line);
		temp =  getFirstWord(line + index);
		mem = temp;
		clearWhiteSpaceFromHead(temp);
		clearWhiteSpaceFromTail(temp);
		if(startWith(temp, ".data") || startWith(temp, ".string")){
			free(mem);
			return True;
		}
	}
	free(temp);
	return False;
}

static char *getNextValidAsmInstructLine(){
	/*This function return the next line that in not empty and not a comment*/
	/*In this function, no need to release memory, once you recall the function, the privious asm instruction will be deleted*/
	static char *mem = NULL;
	static boolean flag = False;
	char *line, *temp;

	if(flag)
		free(mem);

	while (isNextLineValid())
	{
		line = getNextLine();
		clearWhiteSpaceFromHead(line);
		clearWhiteSpaceFromTail(line);

		if(!(*line == 0 || *line == ';')){
			temp =  allocatCpyStr(line);
			replaceFirst(line, ';', 0);			/*Igmnore comments*/
			mem = temp;
			flag = True;
			return temp;
		}
	}
	flag = False;
	return NULL;
}

static List *getFirstLineStartWithCmd(char *line, boolean set_first_index){
	char *cmd, *op1, *op2, **all_op;
	int index = getNextWordWhiteSpaceIndex(line);

	cmd_or_var_flag = True;
	/*Set index 0 to be empty string in the label place*/
	if(set_first_index)
		appendToList(valid_line_struct, "", True);

	/*Split the line into different strings*/
	all_op = split(line + index, ',');

	/*Set index 1 to be cmd string*/
	cmd = getFirstWordClean(line);
	appendToList(valid_line_struct, cmd, True);
	free(cmd);

	/*Set index 2 to be left operand name (if possible)*/
	if(*all_op[0] != 0){
		op1 = getFirstWordClean(all_op[0]);

		if(*op1 == '#')
			appendToList(valid_line_struct, op1+1, True);
		else
			appendToList(valid_line_struct, op1, True);
		free(op1);

		/*Set index 3 to be right operand name (if possible)*/
		if(*all_op[1] != 0){
			op2 = getFirstWordClean(all_op[1]);
			if(*op2 == '#')
				appendToList(valid_line_struct, op2+1, True);
			else
				appendToList(valid_line_struct, op2, True);
			free(op2);
		}
		else
			appendToList(valid_line_struct, "", True);
	}
	else
	{
		appendToList(valid_line_struct, "", True);
		appendToList(valid_line_struct, "", True);
	}
	return valid_line_struct;
}

static List *getFirstLineStartWithLableDec(char *line){
	/*This function return the line structure for Lable call*/
	char *my_line = line, *temp;
	int index;

	cmd_or_var_flag = True;
	/*Check if lable if set with command on the same line or if they spread apart*/
	while (isLineStartsWithLabelDec(my_line) || isLineStatsWithCmd(my_line))
	{
		/*This is will never be true on the firt iteration*/
		/*it is for case that labe set and instruction is set in the following line.*/
		if(isLineStatsWithCmd(my_line))
			return getFirstLineStartWithCmd(my_line, False);

		clearList(valid_line_struct);			/*For case we have a dummy lables*/
		temp = getFirstWordClean(my_line);
		replaceFirst(temp, ':', 0);
		appendToList(valid_line_struct, temp, True);
		free(temp);

		/*If the cmd in the same line of the lable, this statment will be issued*/
		index = getNextWordWhiteSpaceIndex(my_line);
		temp = getFirstWordClean(my_line + index);
		if(*temp != 0){
			free(temp);
			return getFirstLineStartWithCmd(my_line + index, False);
		}

		/*Get the next line to see if the instructions are there.*/
		my_line = getNextValidAsmInstructLine();
	}
	return valid_line_struct;
}

static List *getFirstLineStartWithVarDec(char *line){
	char **all_vars, *new_line, *temp =  getFirstWordClean(line);
	int index = getNextWordWhiteSpaceIndex(line);

	cmd_or_var_flag = False;
	/*Set index zero to be variable name*/
	replaceFirst(temp, ':', 0);
	appendToList(valid_line_struct, temp, True);
	free(temp);

	/*Set index 1 to be the type of the data*/
	temp = getFirstWordClean(line + index);
	appendToList(valid_line_struct, temp, True);
	free(temp);

	/*Split the line into different strings*/
	index += getNextWordWhiteSpaceIndex(line + index);
	all_vars = split(line + index, ',');
	index = 0;

	while (*all_vars[index])
	{
		temp = getFirstWordClean(all_vars[index++]);
		appendToList(valid_line_struct, temp, True);
		free(temp);
	}

	/*This is fot the case that we have matrix decleration.*/
	/*Therefor, variable name may spreat on multipule lines*/
	while (isNextLineValid())
	{
		new_line = getNextValidAsmInstructLine();
		temp = getFirstWordClean(new_line);
		if(startWith(temp, ".data") || startWith(temp, ".string")){
			index = getNextWordWhiteSpaceIndex(new_line);
			all_vars = split(new_line + index, ',');
			index = 0;

			while (*all_vars[index])
			{
				temp = getFirstWordClean(all_vars[index++]);
				appendToList(valid_line_struct, temp, True);
				free(temp);
			}
		}
		else
		{
			pointToPreLineOnFile();
			break;
		}
	}
	return valid_line_struct;
}
