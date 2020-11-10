#include<stdio.h>
#include"Headers\\CheckFile.h"

boolean isFileCompiled(char *file_path){
	char *ptr;
	int line_counter = 1;
	boolean flag = True;

	/*Check if file can be opened*/
	if(openAsmFile(file_path) == False){
		fprintf(stderr, "Failed to open file <%s>\n", file_path);
		return False;
	}

	/*Check if the next line is availble*/
	while (isNextLineValid())
	{
		/*call the next line in the file*/
		ptr = getNextLine();

		/*Check if line passed the syntax check*/
		if(checkSyntax(ptr) == LineIligal){
			fprintf(stderr, "Error on line:%d  \t:: %s\n", line_counter, getError());
			flag = False;
		}
		line_counter++;
	}

	/*Check if there is any variable error or lable errors*/
	if(thereIsVarAndLableErrors()){
		flag = False;
	}

	if(flag)
		fprintf(stdout, "compilation finished succesfuly.\n");
	else
		fprintf(stderr, "compilation failed!\n");

	return flag;
}

