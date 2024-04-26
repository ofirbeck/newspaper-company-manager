#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"


void generalArrayFunction(void* arr, int numOfElements, int sizeOfElement, void f(void* element))
{
	void* element;
	for (int i = 0; i < numOfElements; i++)
	{
		element = (char*)arr + i * sizeOfElement;
		f(element);
	}
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}

char* getStrExactName(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	if(strcmp(msg, "") != 0)
		printf("%s\t", msg);
	myGets(temp, MAX_STR_LEN);

	str = getDynStr(temp);
	return str;
}


char* myGets(char* buffer, int size)
{
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, stdin);
		} while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
		if (ok)
		{
			char* back = buffer + strlen(buffer);
			//trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char* fMyGets(FILE* fp, char* buffer, int size)
{
	char* ok;
	if (buffer != NULL && size > 0 && fp != NULL)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, fp);
		} while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
		if (ok)
		{
			char* back = buffer + strlen(buffer);
			//trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}



int menu(const char** strOptions, const int numOfOptions)
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < numOfOptions; i++)
		printf("%d - %s\n", i, strOptions[i]);
	printf("%d - Quit\n", EXIT);	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}


char* getPersonName(char* type)
{
	char* name;
	while (1)
	{
		printf("Enter %s's name, it should consist of less(or exactly) than %d letters\n", type, MAX_PERSON_NAME_LEN);
		name = getStrExactName("");
		if (CHECK_VALID_PERSON_NAME(name))
			break;
	}
	return name;
}
