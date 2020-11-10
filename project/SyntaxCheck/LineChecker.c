#include<stdio.h>
#include<stdlib.h>
#include"Headers\\LineChecker.h"

/*This file check for all types of line to be comapatible to the Maman demends*/
#define MAX_VAR 31
#define ERROR1 "Variable name can't start with digit"
#define ERROR2 "Variable name contain iligal characters"
#define ERROR3 "Variable name can't be as CPU register names"
#define ERROR4 "Cant have two comma in a raw withou any value batween"
#define ERROR5 ".data type can contain only number!"
#define ERROR6 ".string type must start and end with quote!"
#define ERROR7 "Left operand, must be number with # before or register name or ligal variable name"
#define ERROR8 "This command must get excectly 2 parameters!"
#define ERROR9 "Right operand can be only legal variable name or register"
#define ERROR10 "Left & Right operand must be variable or register"
#define ERROR11 "This command must get excectly 1 parameters!"
#define ERROR12 "This command must get only register or legal variable!"
#define ERROR13 "Comma cant be before a number or after all numbers"
#define ERROR14 "Jumping commands must get legal lable name."
#define ERROR15 "This operation shouldn't get any values."
#define ERROR16 "Variable/Lable can't have the same name of a command!"
#define ERROR17 "You are tyring to redefine variable that was already declaired"
#define ERROR18 "This Lable was already declaired, system dont know which one to choose!"
#define ERROR19 "Command name those not exist!"
#define ERROR20 "Variable/Lable name cant contain over 31 chararcters!"

static boolean isLineStartWithLegalCommand(char *line);
static boolean isLineStartWithVarDecleration(char *line);
static boolean isLineStartWithLabelDecleration(char *line);
static boolean isLineDummyLine(char *line);
static boolean isLineStartWithNote(char *line);
static boolean isLigalVarName(char *var_name, int length);
static boolean isCommandLigal(char *command);
static int getCommandType(char *command, int *index);
static int getCommandIndex(char *command);
static boolean commandType1(char *command, int index);
static boolean commandType2(char *command, int index);
static boolean commandType3(char *command, int index);
static boolean commandType4(char *command, int index);
static boolean isVarDeclerationLigal(char *command);
static void allocateEmptyListForVarAndLable();

char *errorMessage = 0;						/*Pointer to ERRORxx message in case of an error*/
static boolean allocation_flag = True;		/*Allocation of lists (this is enables once for each asm file)*/
List *var_use_list, *var_declaration_list, *label_use_list, *label_declaration_list, *res, *commands;

/*Public functions*/
syntax checkSyntax(char *line){
	/*In case there is variable decleration from previos line it may continue on this one as well under the same name*/
	static boolean var_data_flag = False;
	char *ptr;

	if(allocation_flag){
		allocateEmptyListForVarAndLable();
		allocation_flag = False;
	}
	
	/*Handle the case when there is a decleration of variable in multiple lines.*/
	if(var_data_flag){
		if(isVarDeclerationLigal(line))
			return LineLigal;
		else
			var_data_flag = False;
	}

	/*Check if it is empty line*/
	if(isLineDummyLine(line))
		return LineLigal;

	/*Check if it is note line*/
	if(isLineStartWithNote(line))
		return LineLigal;

	/*Remove any comment from the line*/
	replaceFirst(line, ';', 0);

	/*Check if it is variable decleration line*/
	if(isLineStartWithVarDecleration(line)){
		ptr = getFirstWord(line);
		replaceFirst(ptr, ':', 0);
		if(!isLigalVarName(ptr, strlen(ptr)))
			return LineIligal;
		if(isItemInList(var_declaration_list, ptr)){
			errorMessage = ERROR17;
			return LineIligal;
		}
		appendToList(var_declaration_list, ptr, False);
		clearFirstWord(line);
		free(ptr);
		if(isVarDeclerationLigal(line)){
			var_data_flag = True;
			return LineLigal;
		}
	}

	/*Check if it is label call line*/
	if(isLineStartWithLabelDecleration(line)){
		ptr = getFirstWord(line);
		replaceFirst(ptr, ':', 0);
		if(!isLigalVarName(ptr, strlen(ptr)))
			return LineIligal;
		if(isItemInList(label_declaration_list, ptr)){
			errorMessage = ERROR18;
			return LineIligal;
		}
		appendToList(label_declaration_list, ptr, False);
		clearFirstWord(line);
		if(isLineStartWithLegalCommand(line)){
			if(isCommandLigal(line))
				return LineLigal;
			else
				return LineIligal;
		}
		if(isLineDummyLine(line))
			return LineLigal;
		return LineIligal;
	}

	/*Check if it is command line*/
	if(isLineStartWithLegalCommand(line)){
		if(isCommandLigal(line))
			return LineLigal;
	}

	/*in any other case*/
	return LineIligal;
}

char *getError(){
	/*This function return the last known error*/
	return errorMessage;
}


/*Local functions (Private)*/
static boolean isLineStartWithLegalCommand(char *line){
	/*This function check if we start with legyhjual command and there is a space after it and return true/false accordingly*/
	boolean flag;
	int index = 0;
	char *cmd = getFirstWord(line);

	while (cmd[index] == ' ' || cmd[index] == '\t')
	{
		index++;
	}
	clearWhiteSpaceFromTail(cmd);
	flag = isItemInList(commands, cmd + index);
	free(cmd);

	if(!flag)
		errorMessage = ERROR19;
	return flag;
}

static boolean isLineStartWithVarDecleration(char *line){
	/*This function check if we start with variable declearation*/
	int index = getNextWhiteSpaceIndex(line);
	char *data_type;
	clearWhiteSpaceFromHead(line);
	if(index == -1 || index == 0)
		return False;
	if(*(line + index - 1) != ':')
		return False;

	data_type = getFirstWord(line + index);
	clearWhiteSpaceFromHead(data_type);
	clearWhiteSpaceFromTail(data_type);
	if(strcmp(data_type, ".data") == 0 || strcmp(data_type, ".string") == 0)
		return True;
	return False;
}

static boolean isLineStartWithLabelDecleration(char *line){
	/*This function check if we start with label declearation*/
	int index = getNextWhiteSpaceIndex(line);
	clearWhiteSpaceFromHead(line);
	if(index == -1 || index == 0)
		return False;

	if(*(line + index - 1) == ':' && (*(line + index) == ' ' || *(line + index) == '\t'))
		return True;

	return False;
}

static boolean isLineDummyLine(char *line){
	/*This function check if line is empty*/
	clearWhiteSpaceFromHead(line);
	if(*line == 0)
		return True;
	return False;
}

static boolean isLineStartWithNote(char *line){
	clearWhiteSpaceFromHead(line);
	if(*line == ';')
		return True;
	return False;
}

static boolean isLigalVarName(char *var_name, int length){
	/*This function recieve a variable name and return if the variable name is ligal or not*/
	char temp[] = "r0", *temp_var_name;
	int i = 0;

	if(strlen(var_name) > MAX_VAR){
		errorMessage = ERROR20;
		return False;
	}

	/*Check if it have a command name*/
	temp_var_name = (char*)malloc(length * sizeof(char*));
	if (temp_var_name == NULL)
		exit(0);
	strncpy(temp_var_name, var_name, length);
	temp_var_name[length] = 0;
	if(isItemInList(commands, temp_var_name)){
		errorMessage = ERROR16;
		return False;
	}

	/*Check if it is got the same name of the CPU registers*/
	if(startWith(var_name, "PSW ")){
		errorMessage = ERROR3;
		return False;
	}
	for(i = 0; i < 8; i++){
		temp[1] = '0' + i;
		if(startWith(var_name, temp) && *(var_name + 2) == ' '){
			errorMessage = ERROR3;
			return False;
		}
	}

	/*Check if the variable name dont contain any iligal characters*/
	if(isdigit((int)*var_name)){
		errorMessage = ERROR1;
		return False;
	}
	while (length--)
	{/*Check if it conatain only <a-z> OR <A-Z> OR <0-9>*/
		if(!(isalnum((int)var_name[length]))){
			errorMessage = ERROR2;
			return False;
		}
	}
	return True;
}

static boolean isCommandLigal(char *command){
	int index;
	int type = getCommandType(command, &index);
	switch (type)
	{
		case 1: return commandType1(command, index); break;
		case 2: return commandType2(command, index); break;
		case 3: return commandType3(command, index); break;
		case 4: return commandType4(command, index); break;
	}
	return False;
}

static int getCommandType(char *command, int *index){
	/*This function return the type of the command plus it is updating the index to be the command index value.*/
	char *command_name = getFirstWord(command);
	*index = getCommandIndex(command);
	free(command_name);
	if(*index >= 0 && *index <= 4)
		return 1;
	if(*index >= 5 && *index <= 13)
		return 2;
	if(*index == 14 || *index == 15)
		return 3;
	return 4;
}

static int getCommandIndex(char *command){
	int index = 0;
	char *temp = getFirstWord(command);

	clearWhiteSpaceFromTail(temp);
	while (temp[index] == ' ' || temp[index] == '\t')
	{
		index++;
	}
	index = getIndexByValueFromList(commands, temp + index);
	free(temp);
	return index;
}

static boolean commandType1(char *command, int index){
	char **value_list;

	/*Clean the string from spaces*/
	clearFirstWord(command);
	clearWhiteSpaceFromHead(command);
	clearWhiteSpaceFromTail(command);
	value_list = split(command, ',');  /*Split the data by comma*/

	if(getSplitDataSize(value_list) == 2){
			clearWhiteSpaceFromHead(value_list[1]);
			clearWhiteSpaceFromTail(value_list[1]);
			clearWhiteSpaceFromHead(value_list[0]);
			clearWhiteSpaceFromTail(value_list[0]);
	}
	else
	{
		errorMessage = ERROR8;
		return False;
	}
	/*check if command <mov, add, sub> was entered correctly*/
	if(index == 0 || index == 2 || index == 3){
			if(!(isLigalVarName(value_list[1], strlen(value_list[1])))){
				errorMessage = ERROR9;
				return False;
			}
			else
			{
				appendToList(var_use_list, value_list[1], False);
			}

			if(value_list[0][0] == '#' && isValidNum(value_list[0] + 1))
				return True;
			if(isLigalVarName(value_list[0], strlen(value_list[0]))){
				appendToList(var_use_list, value_list[0], False);
				return True;
			}
			errorMessage = ERROR7;
			return False;
	}

	/*check if command <cmp> was entered correctly*/
	if(index == 1){
		if(!(value_list[0][0] == '#' && isValidNum(value_list[0] + 1))){
			if(!(isLigalVarName(value_list[0], strlen(value_list[0])))){
				errorMessage = ERROR7;
				return False;
			}
		}
			
		if(!(value_list[1][0] == '#' && isValidNum(value_list[1] + 1))){
			if(isLigalVarName(value_list[1], strlen(value_list[1]))){
				return True;
			}
		}
		else
		{
			return True;
		}
		errorMessage = ERROR7;
		return False;
	}

	/*check if command <lea> was entered correctly*/
	if(index == 4){
		if(isLigalVarName(value_list[0], strlen(value_list[0])) &&
			isLigalVarName(value_list[1], strlen(value_list[1]))){
			appendToList(var_use_list, value_list[0], False);
			appendToList(var_use_list, value_list[1], False);
			return True;
		}
		errorMessage = ERROR10;
		return False;
	}
	return False;
}

static boolean commandType2(char *command, int index){
	char **value_list;

	/*Clean the string from spaces*/
	clearFirstWord(command);
	clearWhiteSpaceFromHead(command);
	clearWhiteSpaceFromTail(command);
	value_list = split(command, ',');  /*Split the data by comma*/

	if(getSplitDataSize(value_list) != 1){
		errorMessage = ERROR11;
		return False;
	}
	clearWhiteSpaceFromHead(value_list[0]);
	clearWhiteSpaceFromTail(value_list[0]);

	/*check if command <clr, not, inc, dec> was entered correctly*/
	if(index >= 5 && index <= 8){
		if(isLigalVarName(value_list[0], strlen(value_list[0]))){
			appendToList(var_use_list, value_list[0], False);
			return True;
		}
		errorMessage = ERROR12;
		return False;
	}
	/*check if command <jmp> was entered correctly*/
	if(index == 9){
		if(isLigalVarName(value_list[0] + 1, strlen(value_list[0]) - 1) && *value_list[0] == '&'){
			appendToList(label_use_list, value_list[0]+1, False);
			return True;
		}
		return False;
	}
	/*check if command <bne, jsr> was entered correctly*/
	if(index == 10 || index == 11){
		if(**value_list == '&')
			**value_list = ' ';
		clearWhiteSpaceFromHead(*value_list);
		if(isLigalVarName(value_list[0], strlen(value_list[0]))){
			appendToList(label_use_list, value_list[0], False);
			return True;
		}
		errorMessage = ERROR14;
		return False;
	}
	/*check if command <red> was entered correctly*/
	if(index == 12){
		if(isLigalVarName(value_list[0], strlen(value_list[0]))){
			appendToList(var_use_list, value_list[0], False);
			return True;
		}
		errorMessage = ERROR12;
		return False;
	}
	/*check if command <prn> was entered correctly*/
	if(index ==13){
		if(isLigalVarName(value_list[0], strlen(value_list[0]))){
			appendToList(var_use_list, value_list[0], False);
			return True;
		}
		if(value_list[0][0] == '#' && isValidNum(value_list[0] + 1))
			return True;
		errorMessage = ERROR12;
		return False;
	}
	return False;
}

static boolean commandType3(char *command, int index){
	/*This function handle the command <rts, stop>*/

	/*Clean the string from spaces*/
	clearFirstWord(command);
	clearWhiteSpaceFromHead(command);
	clearWhiteSpaceFromTail(command);
	if (*command == 0)
		return True;
	errorMessage = ERROR15;
	return False;
}

static boolean commandType4(char *command, int index){
	/*This function handle the command <.entry, .extern>*/

	/*Clean the string from spaces*/
	clearFirstWord(command);
	clearWhiteSpaceFromHead(command);
	clearWhiteSpaceFromTail(command);

	/*handle <.entry>*/
	if(index == 16){
		return True;
	}

	/*handle <.extern>*/
	if(index == 17){
		if(isLigalVarName(command, strlen(command))){
			appendToList(var_declaration_list, command, False);
			return True;
		}
	}
	return False;
}

static boolean isVarDeclerationLigal(char *command){
	char **value_list;
	int data_type = -1;

	/*Check what type of data was declaired*/
	clearWhiteSpaceFromHead(command);
	if(startWith(command, ".data "))
		data_type = 0;
	if(startWith(command, ".string "))
		data_type = 1;

	if(data_type == -1)
		return False;

	/*Clean the string from spaces*/
	clearFirstWord(command);
	clearWhiteSpaceFromHead(command);
	clearWhiteSpaceFromTail(command);

	if(*command == ',' || *(command + strlen(command) - 1) == ','){
		errorMessage = ERROR13;
		return False;
	}

	value_list = split(command, ',');  /*Split the data by comma*/

	/*clean each data and check it*/
	while (**value_list)
	{
		/*Clean the data*/
		clearWhiteSpaceFromHead(*value_list);
		clearWhiteSpaceFromTail(*value_list);
		if(**value_list == 0){
			errorMessage = ERROR4;
			return False;
		}
		if(data_type == 0){
			/*Check that we got a valid number*/
			if(isValidNum(*value_list) == False){
				errorMessage = ERROR5;
				return False;
			}
		}

		if(data_type == 1){
			/*Check that the string is start and end with <"> character */
			if(!(**value_list == '"' && *(*value_list + strlen(*value_list) - 1) == '"')){
				errorMessage = ERROR6;
				return False;
			}
		}
		*value_list++;
	}
	return True;
}

static void allocateEmptyListForVarAndLable(){
	/*This function setting all lists of variables and lables to be set empty.*/
	int i;
	char temp[] = "r0";
	char str_commands[][8] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", ".entry", ".extern", ""};
	var_use_list = createList();
	var_declaration_list = createList();
	label_use_list = createList();
	label_declaration_list = createList();
	res = createList();
	commands = createList();

	for(i = 0; i < 8; i++){
		temp[1] = i + '0';
		appendToList(var_declaration_list, temp, False);
	}
	appendToList(var_declaration_list, "PSW", False);

	i = 0;
	while (*str_commands[i])
	{
		appendToList(commands, str_commands[i++], False);
	}
}

boolean thereIsVarAndLableErrors(){
	int i;
	char message[] = "> is used but his defenition no where to be found!";
	char cpu_reg[] = "r0";
	char *temp_ptr;

	/*Remove all declaired vaiable from the list of use variables*/
	for(i = 0; i < sizeOfList(var_declaration_list);i++)
		removeItemByValueFromList(var_use_list, getItemFromList(var_declaration_list, i));

	for(i = 0; i < 8; i++){
		cpu_reg[1] = i + '0';
		removeItemByValueFromList(var_use_list, cpu_reg);
	}

	/*Remove all declaired lables from the list of use lables*/
	for(i = 0; i < sizeOfList(label_declaration_list);i++)
		removeItemByValueFromList(label_use_list, getItemFromList(label_declaration_list, i));

	/*Check if variable name and lable name was declaired the under the same name*/
	for(i = 0; i < sizeOfList(label_declaration_list);i++)
		if(isItemInList(var_declaration_list, getItemFromList(label_declaration_list, i))){
			temp_ptr = strMerge(3, "Variable and Lables cant have the same name! name <", getItemFromList(label_declaration_list, i), "> is conflicted!");
			removeItemByValueFromList(label_use_list, getItemFromList(label_declaration_list, i));
			removeItemByValueFromList(var_use_list, getItemFromList(label_declaration_list, i));
			appendToList(res, temp_ptr, True);
			free(temp_ptr);
		}

	/*Write the error for each variable*/
	for(i = 0; i < sizeOfList(var_use_list);i++){
		temp_ptr = strMerge(3, "Variable <", getItemFromList(var_use_list, i), message);
		appendToList(res, temp_ptr, True);
		free(temp_ptr);
	}

	/*Write the error for each lable*/
	for(i = 0; i < sizeOfList(label_use_list);i++){
		temp_ptr = strMerge(3, "Lable <", getItemFromList(label_use_list, i), message);
		appendToList(res, temp_ptr, True);
		free(temp_ptr);
	}

	clearList(label_declaration_list);
	clearList(var_declaration_list);

	/*Check if there is no undecleared variables/lables or conflicted names*/
	if(sizeOfList(res) == 0)
		return False;
	
	/*If there is an errors, print them*/
	for(i = 0; i < sizeOfList(res);i++)
		fprintf(stderr, "%s\n", getItemFromList(res, i));

	clearList(res);
	allocation_flag = False;
	return True;
}
