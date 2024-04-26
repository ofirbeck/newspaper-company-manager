#pragma once

typedef unsigned char BYTE;

#define MAX_PERSON_NAME_LEN 31
#define MAX_STR_LEN 255
#define EXIT	-1

#define CHECK_RETURN_ZERO(ptr) {if(!ptr) return 0;}
#define CHECK_RETURN_NULL(ptr) {if(!ptr) return NULL;}
#define CLOSE_RETURN_ZERO(fp) {fclose(fp); return 0;}

#define CHECK_CLOSE_RETURN_ZERO(ptr,fp) {if(!ptr){fclose(fp); return 0;}}
#define CHECK_FREE_RETURN_ZERO(object) {free(object); return 0;}

#define CHECK_VALID_PERSON_NAME(name)	(strlen(name) <= (MAX_PERSON_NAME_LEN))

void generalArrayFunction(void* arr, int numOfElements, int sizeOfElement, void f(void* element));
char* getDynStr(char* str);
char* getStrExactName(const char* msg);
char* myGets(char* buffer, int size);
char* fMyGets(FILE* fp, char* buffer, int size);
int menu(const char** strOptions, const int numOfOptions);
char* getPersonName(char* type);