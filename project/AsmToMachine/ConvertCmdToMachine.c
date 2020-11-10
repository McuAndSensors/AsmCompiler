#include<stdio.h>
#include<stdlib.h>
#include"Headers\\ConvertCmdToMachine.h"

#define INSTRUCT_SIZE 24
#define EXT "ext"
#define ENT "ent"
#define OB "ob"

static void reset();
static void exportMachineFiles(char *file_path, char *file_name);
static void allocateInstructionWord();
static void attched_all_data();
static void secondRun();
static void firstRun();
static void updateVarMemLocation();
static void updateEntryListOfVariables();
static char *extructFileName(char *file_path_with_name);
static char *extructFilePath(char *file_path_with_name);
static void cmdOrLabelDecHandlerFirstRun(List *op_address, List *splited_cmd_line);
static void setDataOfInstruction(char *op, int data_index);
static void cmdOrLabelDecHandlerSecondRun(List *op_address, List *splited_cmd_line);
static void varDecHandlerFirstRun(List *splited_cmd_line);

/*This file is converting the command into the machine file and calculate the word following the instruction*/

/*All variables available for this file functions only*/
List *list_extern, *list_entry;			/*Variable and lables (names only) of entry and extern storage*/
List *list_extern_res, *list_entry_res;	/*Final entry * extern in file structute storage*/
List *list_var_name, *list_var_address;	/*Contain the column of name and address, index of list behave like a row of matrix*/
List *list_lable_name, *list_lable_address;	/*Contain the column of name and address, index of list behave like a row of matrix*/
List *ps_name, *ps_address;					/*Contain the column of name and address, index of list behave like a row of matrix*/
List *list_var_data;						/*Contain all variable data in the asm file declair order*/
List *list_final_res_address;				/*Final data structure ready to write to *.ob file in address column*/
List *list_final_res_value;					/*Final data structure ready to write to *.ob file in value column*/
int pc, ic, var_address, instruction_amount;/*pc & ic values relevant to all functions to update accordingly*/
int *cmd_word_size;							/*Ecach index contain the size of words following the instructions size. size can be 0/1/2 word in addition to instruction*/
InstructionsWord *inst;						/*contain the value of the instruction*/

static void varDecHandlerFirstRun(List *splited_cmd_line){
	/*This function calculate the total data size that we have and put them into a list*/
	char *data_type = getItemFromList(splited_cmd_line, 1);
	char *data;
	int i = 0;

	appendToList(list_var_name, getItemFromList(splited_cmd_line, 0), True);
	appendToList(list_var_address, convertIntToStr(var_address), True);

	/*Add all the variable chars data into the list*/
	if(startWith(data_type, ".string")){
		var_address += strlen(getItemFromList(splited_cmd_line, 2)) - 1;
		data = getItemFromList(splited_cmd_line, 2);
		i++;
		while (data[i+1])
		{
			appendToList(list_var_data, convertIntToStr(data[i]), True);
			i++;
		}
		appendToList(list_var_data, "0", True);
	}

	/*Add all the variable numbers data into the list*/
	if(startWith(data_type, ".data")){
		var_address += sizeOfList(splited_cmd_line) - 2;
		i = 2;
		while (sizeOfList(splited_cmd_line) > i)
		{
			appendToList(list_var_data, getItemFromList(splited_cmd_line, i), True);
			i++;
		}
	}
}

static void cmdOrLabelDecHandlerSecondRun(List *op_address, List *splited_cmd_line){
	/*This function path on all command again and fill all the missing data in the second & thired word*/

	char *cmd = getItemFromList(splited_cmd_line, 1);
	char *lable = getItemFromList(splited_cmd_line, 0);
	char *op1 = getItemFromList(splited_cmd_line, 2);
	char *op2 = getItemFromList(splited_cmd_line, 3);
	char *temp_ptr1, *temp_ptr2; 

	/*This part was alread handled in the first run*/
	if(startWith(cmd, ".extern") || startWith(cmd, ".entry")){
		return;
	}

	/*Update PC and IC*/
	ic++;
	pc++;

	/*Update entry of lable to list for *ent file.*/
	if(isItemInList(list_entry, lable)){
		temp_ptr1 = convertIntToStr(pc);
		temp_ptr2 = strMerge(3, lable, "\t", temp_ptr1);
		appendToList(list_entry_res, temp_ptr2, True);
		free(temp_ptr1);
		free(temp_ptr2);
	}

	/*If instruction word size is 1, then it can be op1 or op2*/
	if(cmd_word_size[ic-1] == 1){
		if(getRegisterNum(op1) == -1)
			setDataOfInstruction(op1, 0);
		else
			setDataOfInstruction(op2, 0);
	}

	/*If the instruction word is in size 2, both operand must be handled and get the data word value*/
	if(cmd_word_size[ic-1] == 2){
		setDataOfInstruction(op1, 0);
		setDataOfInstruction(op2, 1);
	}
}

static void updateEntryListOfVariables(){
	/*This function updating all .entry command*/
	int index = sizeOfList(list_entry), var_index;
	char *ptr, *res_ptr;
	while (index--)
	{
		/*Build the list for the file of entry in the structure of {EntryVariableOrLable		Address}*/
		ptr = getItemFromList(list_entry, index);
		if(isItemInList(list_var_name, ptr)){
			var_index = getIndexByValueFromList(list_var_name, ptr);
			res_ptr = strMerge(3, ptr, "\t", getItemFromList(list_var_address, var_index));
			appendToList(list_entry_res, res_ptr, True);
			free(res_ptr);
		}
	}
}

static void setDataOfInstruction(char *op, int data_index){
	/*This function is checking what type of opearnd we have and updating it accordingly*/
	int index, jump_calc;
	char *ptr;
	pc++;

	((inst[ic -1]).Data[data_index]).A = 0;
	((inst[ic -1]).Data[data_index]).E = 0;
	((inst[ic -1]).Data[data_index]).R = 0;

	/*Handle the case of imidiate number*/
	if(isValidNum(op)){
		((inst[ic -1]).Data[data_index]).data = convertStrToInt(op);
		((inst[ic -1]).Data[data_index]).A = 1;
	}

	/*Handle the case of internal variable*/
	if(isItemInList(list_var_name, op)){
		index = getIndexByValueFromList(list_var_name, op);
		((inst[ic -1]).Data[data_index]).data = convertStrToInt(getItemFromList(list_var_address, index));
		((inst[ic -1]).Data[data_index]).R = 1;
	}

	/*Handle the case of lable jmp in the case of <&Lable>*/
	if(*op == '&' && isItemInList(list_lable_name, op + 1)){
		index = getIndexByValueFromList(list_lable_name, op+1);
		jump_calc = convertStrToInt(getItemFromList(list_lable_address, index)) - pc + 1;
		((inst[ic -1]).Data[data_index]).data = jump_calc;
		((inst[ic -1]).Data[data_index]).A = 1;
	}

	/*Handle the case of lable jmp in the case of <Lable>*/
	if(isItemInList(list_lable_name, op)){
		index = getIndexByValueFromList(list_lable_name, op);
		((inst[ic -1]).Data[data_index]).data = convertStrToInt(getItemFromList(list_lable_address, index));
		((inst[ic -1]).Data[data_index]).R = 1;
	}

	/*Handle the case of external variable*/
	if(isItemInList(list_extern, op)){
		appendToList(ps_name, op, True);
		appendToList(ps_address, convertIntToStr(pc), True);
		((inst[ic -1]).Data[data_index]).data = 0;
		((inst[ic -1]).Data[data_index]).E = 1;

		/*Update the variable adress and name for the *.ent file*/
		ptr = strMerge(3, op, "\t", convertIntToStr(pc));
		appendToList(list_extern_res, ptr, True);
		free(ptr);
	}
}

static void cmdOrLabelDecHandlerFirstRun(List *op_address, List *splited_cmd_line){
	int data_command_amount = 0;
	char *cmd = getItemFromList(splited_cmd_line, 1);

	if(startWith(cmd, ".extern")){
		appendToList(list_extern, getItemFromList(splited_cmd_line, 2), False);
		return;
	}

	if(startWith(cmd, ".entry")){
		appendToList(list_entry, getItemFromList(splited_cmd_line, 2), False);
		return;
	}

	ic++;
	pc++;
	allocateInstructionWord();

	/*Check if the register adress is different then 3, which mean, more data instruction is needed*/
	if(*getItemFromList(op_address, 0) != '3' && (numOfOperands(cmd) == 1 || numOfOperands(cmd) == 2))
		data_command_amount++;

	if(*getItemFromList(op_address, 1) != '3' && numOfOperands(cmd) == 2)
		data_command_amount++;

	/*Check if lable cell is not empty, if so, add the lable to the list and add his address*/
	if(*getItemFromList(splited_cmd_line, 0) != 0){
		appendToList(list_lable_name, getItemFromList(splited_cmd_line, 0), True);
		appendToList(list_lable_address, convertIntToStr(pc), True);
	}

	pc += data_command_amount;
	cmd_word_size[ic-1] = data_command_amount;		/*Update data size of the word */

	(inst[ic -1]).A = 1;
	(inst[ic -1]).R = 0;
	(inst[ic -1]).E = 0;

	/*Set the data instruction fields*/
	(inst[ic -1]).opcode = getCmdOpcode(cmd);
	
	(inst[ic -1]).op1_adress = *getItemFromList(op_address, 0) - '0';
	(inst[ic -1]).op1_register = getRegisterNumOrZero(getItemFromList(splited_cmd_line, 2));

	(inst[ic -1]).op2_adress = *getItemFromList(op_address, 1) - '0';
	(inst[ic -1]).op2_register = getRegisterNumOrZero(getItemFromList(splited_cmd_line, 3));

	(inst[ic -1]).function = getCmdFunction(cmd);

	if(numOfOperands(cmd) == 1){
		/*In case of only one operand, value of the only opearnd should be set on the right side of the word*/
		(inst[ic -1]).op1_adress = 0;
		(inst[ic -1]).op1_register = 0;

		(inst[ic -1]).op2_adress = *getItemFromList(op_address, 0) - '0';
		(inst[ic -1]).op2_register = getRegisterNumOrZero(getItemFromList(splited_cmd_line, 2));
	}
}

static void firstRun(){
	/*Make the first run*/
	List *split_line, *op_list;
	ic = 0;
	pc = 99;
	do{
		/*Get the command splitted into parts of [Lable, Cmd, Operand1, Operand2]*/
		split_line = getNextSplitedCmdLine();
		if(sizeOfList(split_line) != 0){
			if(SplitedLineIsNotVarDec()){
				/*Get the address of each operand. value can be 0/1/2/3*/
				op_list = getOpearndAdress(getItemFromList(split_line, 1), getItemFromList(split_line, 2), getItemFromList(split_line, 3));

				/*Update the size word requiere for each instruction and build the instruction word according to given command*/
				cmdOrLabelDecHandlerFirstRun(op_list, split_line);
			}
			else
				varDecHandlerFirstRun(split_line);	/*Calculate the variable size and append them into the lists*/
		}
	}
	while(sizeOfList(split_line) != 0);

	/*Update the memory location of variables only*/
	updateVarMemLocation();
}

static void updateVarMemLocation(){
	/*This function is updating the memory location of variable after all program instruction was allocated*/
	int index = sizeOfList(list_var_address), var_mem_location;
	char *new_str_address;
	
	/*If there is no variables, no need to do nothing*/
	if(index == 0)
		return;

	index = 0;
	/*Go through each address, get the number value, add to it the pc, remove the old value and update the new value*/
	while (sizeOfList(list_var_address) > index)
	{
		var_mem_location = convertStrToInt(getItemFromList(list_var_address, 0));
		removeItemByIndexFromList(list_var_address, 0);
		new_str_address = convertIntToStr(var_mem_location + pc + 1);
		appendToList(list_var_address, new_str_address, True);
		free(new_str_address);
		index++;
	}
}

static void secondRun(){
	/*Make the second run*/
	List *split_line, *op_list;
	ic = 0;
	pc = 99;

	/*Upadte the true address of the variables or lables adress*/
	updateEntryListOfVariables();
	do{
		/*Get the command splitted into parts of [Lable, Cmd, Operand1, Operand2]*/
		split_line = getNextSplitedCmdLine();
		if(sizeOfList(split_line) != 0){
			if(SplitedLineIsNotVarDec()){
				/*Get the address of each operand. value can be 0/1/2/3*/
				op_list = getOpearndAdress(getItemFromList(split_line, 1), getItemFromList(split_line, 2), getItemFromList(split_line, 3));

				/*Update all variable location and lable location that were found on first run*/
				cmdOrLabelDecHandlerSecondRun(op_list, split_line); 
			}
		}
	}
	while(sizeOfList(split_line) != 0);
}

void convertAsmFileAndExportRes(char *file_path){
	/*Run the whole proccess of converting the asm file to machine code and export all 3 files*/
	char *ptr_path, *ptr_name;
	setCompieldFilePath(file_path);	/*update file to be converted*/
	firstRun();						/*Make the first run*/
	closeAsmFile();					/*Close the file*/

	/*Same process for the second run*/
	setCompieldFilePath(file_path);
	secondRun();
	closeAsmFile();
	
	/*Combine all values togther ready for export*/
	attched_all_data();

	/*Get file name and path for the export*/
	ptr_path = extructFilePath(file_path);
	ptr_name = extructFileName(file_path);

	/*Export all 3 files*/
	exportMachineFiles(ptr_path, ptr_name);
	free(ptr_path);
	free(ptr_name);
	reset();
}

static char *extructFileName(char *file_path_with_name){
	/*This function find the name of the file and returing it*/
	int index = 0, end_of_path = 0, point_index = 0;
	char *ptr, *temp_ptr;

	/*Find the last \\ of the path*/
	while (file_path_with_name[index])
	{
		if(file_path_with_name[index] == '\\')
			end_of_path = index + 1;
		index++;
	}

	/*Find the point index*/
	index = 0;
	while (file_path_with_name[index])
	{
		if(file_path_with_name[index] == '.')
			point_index = index;
		index++;
	}

	/*Allocate string and copy the results into it*/
	temp_ptr = strMerge(1, file_path_with_name);
	temp_ptr[point_index] = 0;
	ptr = strMerge(1, end_of_path + temp_ptr);
	free(temp_ptr);
	return ptr;
}

static char *extructFilePath(char *file_path_with_name){
	/*This file find the path of the file and return it, in case that there is no path, it will return an empty string*/
	int index = 0, end_of_path = 0;
	char *ptr, *temp_ptr;

	/*Find the last \\ of the path*/
	while (file_path_with_name[index])
	{
		if(file_path_with_name[index] == '\\')
			end_of_path = index;
		index++;
	}

	/*If the name has no path, it is should be empty path*/
	if(end_of_path == 0)
		return strMerge(1, "");
	temp_ptr = strMerge(1, file_path_with_name);
	temp_ptr[end_of_path] = 0;
	ptr = strMerge(1, temp_ptr);
	free(temp_ptr);
	return ptr;
}

static void attched_all_data(){
	/*This function unite all instructions data and variable data into lists*/
	/*The lists that this function is building, will be the files output*/
	int local_pc = 99, local_ic = -1, i; 
	unsigned int *inst_word, *data_word;
	char *str_ptr;

	/*Add all commends and instructions to the memory*/
	while ((ic - 1) > local_ic)
	{
		local_ic++;
		local_pc++;

		/*Add the command word to memory*/
		inst_word = (unsigned int*) &(inst[local_ic]);
		*inst_word &= 0xffffff;
		
		str_ptr = convertIntTo24BitHexStr(*inst_word);
		appendToList(list_final_res_value, str_ptr, True);
		free(str_ptr);

		/*Add the command adress to memory*/
		str_ptr = convertIntToStr(local_pc);
		appendToList(list_final_res_address, str_ptr, True);
		free(str_ptr);

		for(i = 0; (i < cmd_word_size[local_ic]) && (ic > local_ic); i++){
			local_pc++;
			/*Add the instruction word to memory*/
			data_word = (unsigned int*) &((inst[local_ic]).Data[i]);
			*data_word &= 0xffffff;
			str_ptr = convertIntTo24BitHexStr(*data_word);
			appendToList(list_final_res_value, str_ptr, True);
			free(str_ptr);

			/*Add the instruction adress to memory*/
			str_ptr = convertIntToStr(local_pc);
			appendToList(list_final_res_address, str_ptr, True);
			free(str_ptr);
		}
	}

	/*Add all variable data to the memory*/
	i = 0;
	while (sizeOfList(list_var_data) > i)
	{
		local_pc++;
		/*Add variable value into the memory*/
		str_ptr = convertIntTo24BitHexStr(convertStrToInt(getItemFromList(list_var_data, i)));
		appendToList(list_final_res_value, str_ptr, True);
		free(str_ptr);

		/*Add variable adress into the memory*/
		str_ptr = convertIntToStr(local_pc);
		appendToList(list_final_res_address, str_ptr, True);
		free(str_ptr);	
		i++;
	}
}

static void allocateInstructionWord(){
	/*This function grow dinemiclly the storage for the commands and the data word size of each one of them*/
	inst = (InstructionsWord*)realloc(inst, (ic + 1) * sizeof(InstructionsWord));
	cmd_word_size = (int*)realloc(cmd_word_size, (ic + 1) * sizeof(int));
	if(cmd_word_size == NULL || inst == NULL){
		fprintf(stderr, "System is out of memory!\n Cant builed ASM files.");
		exit(0);
	}
}

static void exportMachineFiles(char *file_path, char *file_name){
	/*This function write out all 3 output files*/

	/*Build *.ob file*/
	if(fileWRightSetup(file_path, file_name, OB)){
		writeTwoNumAsRaw(pc-99, var_address);
		writeTwoListsAsColumn(list_final_res_address, list_final_res_value);
		stopWriting();
	}

	/*Build *.ext file*/
	if(fileWRightSetup(file_path, file_name, EXT)){
		writeListsAsColumn(list_extern_res);
		stopWriting();
	}

	/*Build *.ent file*/
	if(fileWRightSetup(file_path, file_name, ENT)){
		writeListsAsColumn(list_entry_res);
		stopWriting();
	}
}

void setup(){
	/*This function is allocating all requiered structures/variable for the file convertion*/
	/*In addition, this function call the setup of all sub-function levels*/
	setupCmdInfo();

	ps_name = createList();
	ps_address = createList();
	list_entry = createList();
	list_extern = createList();
	list_entry_res = createList();
	list_extern_res = createList();
	list_var_name = createList();
	list_lable_name = createList();
	list_var_address = createList();
	list_lable_address = createList();
	list_var_data = createList();
	list_final_res_address = createList();
	list_final_res_value = createList();
	pc = 100;
	ic = 0;
	var_address = 0;
	cmd_word_size = (int*)malloc((ic + 1) * sizeof(int));
	inst = (InstructionsWord*)malloc((ic + 1) * sizeof(InstructionsWord));
	if(cmd_word_size == NULL || inst == NULL){
		fprintf(stderr, "System is out of memory!\n Cant builed ASM files.");
		exit(0);
	}
}

static void reset(){
	/*This function reset all data values back to defaule*/
	free(cmd_word_size);
	clearList(ps_name);
	clearList(ps_address);
	clearList(list_extern);
	clearList(list_entry);
	clearList(list_extern_res);
	clearList(list_entry_res);
	clearList(list_var_name);
	clearList(list_lable_name);
	clearList(list_var_address);
	clearList(list_lable_address);
	clearList(list_var_data);
	clearList(list_final_res_address);
	clearList(list_final_res_value);
	pc = 100;
	ic = 0;
	var_address = 0;
	inst = (InstructionsWord*)malloc((ic + 1) * sizeof(InstructionsWord));
	cmd_word_size = (int*)malloc((ic + 1) * sizeof(int));
	if(cmd_word_size == NULL || inst == NULL){
		fprintf(stderr, "System is out of memory!\n Cant builed ASM files.");
		exit(0);
	}
}
