#include<stdio.h>
#include"Headers\\ConvertCmdToMachine.h"
#include"Headers\\CheckFile.h"

boolean isFileTypeLigale(char *file_path){
	/*This function check if the file type is a ligal type*/
	int index = 0;
	while (file_path[index] != 0)
	{
		if(file_path[index++] == '.')
			break;
	}

	/*Check that the file ends with asm*/
	if(file_path[index - 1] == '.'){
		if(file_path[index] == 'A' || file_path[index] == 'a')
			if(file_path[index+1] == 'S' || file_path[index+1] == 's')
				if(file_path[index+2] == 0)
					return True;
		fprintf(stderr, "The file <%s> is not an *.as file!\n", file_path);
		return False;
	}
	else{
		fprintf(stderr, "The string entered <%s> is not any file type!\n", file_path);
		return False;
	}
	return False;
}

void handleFileComplieAndConversion(char *file_path){
	/*This function compile the file, and it is converting the file, only if compilation ends up succesufuly*/
	if(isFileCompiled(file_path)){
		convertAsmFileAndExportRes(file_path);
	}
}


int main(int argc, char *argv[]){
	int index = 1;
	if(argc == 1){
		fprintf(stderr, "No files were passed for the compiler!");
		return -1;
	}
	
	setup();
	while (argc > index)
	{
		printf("Start woking on file:<%s>\n",argv[index]);
		if(isFileTypeLigale(argv[index]))
			handleFileComplieAndConversion(argv[index]);
		index++;
	}
}

