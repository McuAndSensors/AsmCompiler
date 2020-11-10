maman14: MainRunFile.o CmdInfo.o ConvertCmdToMachine.o LineSpliter.o WriteToFile.o StringAdvanceFunc.o ListDataStructute.o FileReadHandler.o LineChecker.o CheckFile.o
	gcc -g -ansi -Wall -pedantic MainRunFile.o CmdInfo.o ConvertCmdToMachine.o LineSpliter.o WriteToFile.o StringAdvanceFunc.o ListDataStructute.o FileReadHandler.o LineChecker.o CheckFile.o -o maman14

CheckFile.o: SyntaxCheck\CheckFile.c  Headers\CheckFile.h
	gcc -c -Wall SyntaxCheck\CheckFile.c -o CheckFile.o -I.
	
LineChecker.o: SyntaxCheck\LineChecker.c  Headers\LineChecker.h
	gcc -c -Wall SyntaxCheck\LineChecker.c -o LineChecker.o -I.
	
FileReadHandler.o: SupportLibreys\FileReadHandler.c  Headers\FileReadHandler.h
	gcc -c -Wall SupportLibreys\FileReadHandler.c -o FileReadHandler.o -I.
	
ListDataStructute.o: SupportLibreys\ListDataStructute.c  Headers\ListDataStructute.h Headers\StringAdvanceFunc.h
	gcc -c -Wall SupportLibreys\ListDataStructute.c -o ListDataStructute.o -I.
	
StringAdvanceFunc.o: SupportLibreys\StringAdvanceFunc.c  Headers\StringAdvanceFunc.h
	gcc -c -Wall SupportLibreys\StringAdvanceFunc.c -o StringAdvanceFunc.o -I.
	
WriteToFile.o: SupportLibreys\WriteToFile.c  Headers\WriteToFile.h
	gcc -c -Wall SupportLibreys\WriteToFile.c -o WriteToFile.o -I.
	
LineSpliter.o: AsmToMachine\LineSpliter.c  Headers\LineSpliter.h
	gcc -c -Wall AsmToMachine\LineSpliter.c -o LineSpliter.o -I.
	
ConvertCmdToMachine.o: AsmToMachine\ConvertCmdToMachine.c  Headers\ConvertCmdToMachine.h
	gcc -c -Wall AsmToMachine\ConvertCmdToMachine.c -o ConvertCmdToMachine.o -I.
	
CmdInfo.o: AsmToMachine\CmdInfo.c  Headers\CmdInfo.h
	gcc -c -Wall AsmToMachine\CmdInfo.c -o CmdInfo.o -I.
	
MainRunFile.o: Main\MainRunFile.c  Headers\CheckFile.h Headers\ConvertCmdToMachine.h
	gcc -c -Wall Main\MainRunFile.c -o MainRunFile.o -I.