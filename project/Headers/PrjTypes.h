#ifndef PRJTYPES_H
#define PRJTYPES_H

typedef enum boolean{False, True} boolean;
typedef enum syntax{LineLigal, LineIligal, NextLineIsRequired} syntax;
typedef struct List
{
	char **head;
	int size;

}List;

typedef struct InstructionDataWord
{
	unsigned int E:1;
	unsigned int R:1;
	unsigned int A:1;
	unsigned int data:21;
	unsigned int :0;

}InstructionDataWord;

typedef struct InstructionsWord
{
	unsigned int E:1;
	unsigned int R:1;
	unsigned int A:1;
	unsigned int function:5;
	unsigned int op2_register:3;
	unsigned int op2_adress:2;
	unsigned int op1_register:3;
	unsigned int op1_adress:2;
	unsigned int opcode:6;
	unsigned int :0;
	InstructionDataWord Data[2];

}InstructionsWord;



#endif

