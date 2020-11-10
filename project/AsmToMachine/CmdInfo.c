#include<stdio.h>
#include"Headers\\CmdInfo.h"

/*This file supply all the commands information. function, opcode, address atc...*/ 
#define IMIDIEATE_ADRESS	"0"
#define DIRECT_ADRESS		"1"
#define RELATIVE_ADRESS		"2"
#define REGISTER_ADRESS		"3"
#define ZERO				"0"

static List *twoOperandHandler(char *cmd, char *operand1, char *operand2);
static List *oneOperandHandler(char *cmd, char *operand1, char *operand2);
static List *zeroOperandHandler(char *cmd, char *operand1, char *operand2);
static void setCmdJumpType();
static void setCmdOpNum();
static void setCmdOpcode();
static void setCmdFunction();
static void setCmdName();

List *column_cmd_mame, *column_cmd_opcode, *column_cmd_function, *column_cmd_op_num, *column_cmd_jmp_type, *res_list;

/*Public functions*/
void setupCmdInfo(){
	/*This function must be call one time before approching to any other function on this file.*/
	res_list = createList();
	setCmdName();
	setCmdFunction();
	setCmdOpcode();
	setCmdOpNum();
	setCmdJumpType();
}

int getCmdFunction(char *cmd){
	/*This function will return the function value of the given command*/
	int x = getIndexByValueFromList(column_cmd_mame, cmd);
	return convertStrToInt(getItemFromList(column_cmd_function, x));
}

int getCmdOpcode(char *cmd){
	/*This function will return the opcode value of the given command*/
	int x = getIndexByValueFromList(column_cmd_mame, cmd);
	return convertStrToInt(getItemFromList(column_cmd_opcode, x));
}

List *getOpearndAdress(char *cmd, char *operand1, char *operand2){
	/*This function return a list that contain the relevant value of operand address for both op1 & op2*/
	/*The values in the list are presented as decinmal value in string presentation*/
	/*The string allocation is handled from this file. any new call will overite the priviose List*/

	clearList(res_list);
	switch (numOfOperands(cmd))
	{
		case 0: return zeroOperandHandler(cmd, operand1, operand2); break;
		case 1: return oneOperandHandler(cmd, operand1, operand2); break;
		case 2: return twoOperandHandler(cmd, operand1, operand2); break;
	}
	return createList();
}

int numOfOperands(char *cmd){
	/*This function will return the value of operands that the command must get excextly*/
	int x = getIndexByValueFromList(column_cmd_mame, cmd);
	return convertStrToInt(getItemFromList(column_cmd_op_num, x));
}

int getRegisterNum(char *operand){
	/*Check if it is a rgister, if so, return its number, otherwise, return -1*/
	if(*operand == 'r' && operand[1] >= '0' && operand[1] < '8' && operand[2] == 0)
		return operand[1] - '0';
	return -1;
}

int getRegisterNumOrZero(char *operand){
	/*Check if it is a rgister, if so, return its number, otherwise, return zero*/
	if(*operand == 'r' && operand[1] >= '0' && operand[1] < '8' && operand[2] == 0)
		return operand[1] - '0';
	return 0;
}

/*Private file functions*/
static List *zeroOperandHandler(char *cmd, char *operand1, char *operand2){
	/*This function return the value of adress for command with no operands*/
	appendToList(res_list, ZERO, True);
	appendToList(res_list, ZERO, True);
	return res_list;
}

static List *oneOperandHandler(char *cmd, char *operand1, char *operand2){
	/*This function return the value of adress for command with one operand type*/

	/*Handle first operand*/
	if(isItemInList(column_cmd_jmp_type, cmd))
		if(*operand1 == '&')/*&Lable*/
			appendToList(res_list, RELATIVE_ADRESS, True);
		else/*Lable*/
			appendToList(res_list, DIRECT_ADRESS, True);

	else if(isValidNum(operand1))/*#Number*/
		appendToList(res_list, IMIDIEATE_ADRESS, True);

	else if(getRegisterNum(operand1) != -1)/*r0/r1/r2/.../r7*/
		appendToList(res_list, REGISTER_ADRESS, True);

	else/*Variable*/
		appendToList(res_list, DIRECT_ADRESS, True);

	/*Second operand is zero*/
	appendToList(res_list, ZERO, True);
	return res_list;
}

static List *twoOperandHandler(char *cmd, char *operand1, char *operand2){
	/*This function return the value of adress for command with two operands type*/

	/*Handle first operand*/
	if(isValidNum(operand1))
		appendToList(res_list, IMIDIEATE_ADRESS, True);

	else if(getRegisterNum(operand1) != -1)
		appendToList(res_list, REGISTER_ADRESS, True);

	else
		appendToList(res_list, DIRECT_ADRESS, True);

	/*Handle second operand*/
	if(isValidNum(operand2))
		appendToList(res_list, IMIDIEATE_ADRESS, True);

	else if(getRegisterNum(operand2) != -1)
		appendToList(res_list, REGISTER_ADRESS, True);

	else
		appendToList(res_list, DIRECT_ADRESS, True);

	return res_list;
}

static void setCmdName(){
	/*This function load the data of all commands to a List the is behaving like a row of big table*/
	int index = 0;
	char str_commands_name[][8] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", ".entry", ".extern", ""};

	column_cmd_mame = createList();
	while (*str_commands_name[index])
	{
		appendToList(column_cmd_mame, str_commands_name[index++], True);
	}
}

static void setCmdFunction(){
	/*This function load the data of all "command function value" to a List that is behaving like a row of big table*/
	int index = 0;
	char str_commands_func[][2] = {"0", "0", "1", "2", "0", "1", "2", "3", "4", "1", "2", "3", "0", "0", "0", "0", "0", "0", ""};

	column_cmd_function = createList();
	while (*str_commands_func[index])
	{
		appendToList(column_cmd_function, str_commands_func[index++], True);
	}
}

static void setCmdOpcode(){
	/*This function load the data of all "command opcode value" to a List that is behaving like a row of big table*/
	int index = 0;
	char str_commands_opcode[][3] = {"0", "1", "2", "2", "4", "5", "5", "5", "5", "9", "9", "9", "12", "13", "14", "15", "0", "0", ""};

	column_cmd_opcode = createList();
	while (*str_commands_opcode[index])
	{
		appendToList(column_cmd_opcode, str_commands_opcode[index++], True);
	}
}

static void setCmdOpNum(){
	/*This function load the data of all "command operands number" to a List that is behaving like a row of big table*/
	int index = 0;
	char str_commands_op_num[][2] = {"2", "2", "2", "2", "2", "1", "1", "1", "1", "1", "1", "1", "1", "1", "0", "0", "0", "0",""};

	column_cmd_op_num = createList();
	while (*str_commands_op_num[index])
	{
		appendToList(column_cmd_op_num, str_commands_op_num[index++], True);
	}
}

static void setCmdJumpType(){
	/*The function load all the jump command into a list. (This list is not part of the table)*/
	column_cmd_jmp_type = createList();
	appendToList(column_cmd_jmp_type, "jmp", False);
	appendToList(column_cmd_jmp_type, "bne", False);
	appendToList(column_cmd_jmp_type, "jsr", False);
}
