#include<string.h>
#include<stdarg.h>
#include<ctype.h>
#include"Headers\\PrjTypes.h"
boolean startWith(char *source, char *start_with_str);
void clearWhiteSpaceFromHead(char *source);
void clearWhiteSpaceFromTail(char *source);
void replaceAll(char *source, char ch1, char to_ch2);
void replaceFirst(char *source, char ch1, char to_ch2);
int getNextWhiteSpaceIndex(char *source);
int getNextWordWhiteSpaceIndex(char *source);
void clearFirstWord(char *source);
char *getFirstWord(char source[]);
char **split(char *str, char ch);
boolean isValidNum(char *str);
int getSplitDataSize(char **list);
int convertStrToInt(char *str);
char *strMerge(int strings_to_merge, ...);
boolean stringComp(char *str1, char *str2);
char *getFirstWordClean(char *str);
char *convertIntToStr(int value);
char *allocatCpyStr(char *str);
void reverseString(char *str);
char *convertIntTo24BitHexStr(int value);