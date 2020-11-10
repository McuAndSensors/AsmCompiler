#include"Headers\\ListDataStructute.h"
#include"Headers\\StringAdvanceFunc.h"
#include"Headers\\LineSpliter.h"

int getRegisterNum(char *operand);
int numOfOperands(char *cmd);
int getCmdOpcode(char *cmd);
int getCmdFunction(char *cmd);
void setupCmdInfo();
int getRegisterNumOrZero(char *operand);
List *getOpearndAdress(char *cmd, char *operand1, char *operand2);
