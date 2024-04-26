#pragma once
#include "Page.h"

#define EDITION_CODE_LENGTH 3
#define MAX_PAGES_IN_NEWSPAPER 100
#define NEWSPAPER_PRINTING_OUTCOME -1

typedef struct {
	char editionCode[EDITION_CODE_LENGTH + 1];
	Page** allPages;
	int pageCount;
}Newspaper;

int initNewspaper(Newspaper* pNewspaper, char uniqueCode[EDITION_CODE_LENGTH + 1]);
void getEditionCode(char* code);
int getNumOfTitles(const Newspaper* pNewspaper);
void updateTitleArrFromNewspaper(const Newspaper* pNewspaper, char*** titles, int* countOfTitles);
int addPages(Newspaper* pNewspaper, int isNewNewspaper);
int editPage(Page* pPage, PageObjectStock* stock, Newspaper* pNewspaper);
int editPagesInNewspaper(Newspaper* pNewspaper, PageObjectStock* stock);
int writeNewspaperToTextFile(const Newspaper* pNewspaper, FILE* fp, int toPublish);
int writeNewspaperToBinaryFile(const Newspaper* pNewspaper, FILE* fp);
int readNewspaperFromTextFile(Newspaper* pNewspaper, FILE* fp);
int readNewspaperFromBinaryFile(Newspaper* pNewspaper, FILE* fp);
int getNewspaperIncomeOrOutcome(const Newspaper* pNewspaper);
void limitedPrintNewspaper(const Newspaper* pNewspaper);
void freeNewspaper(Newspaper* pNewspaper);