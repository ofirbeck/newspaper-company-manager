#pragma once
#include "PageObjectStock.h"
#include "General.h"

#define MAX_PAGEOBJECTS_PER_PAGE 5

typedef enum
{
	eAddArticleToPage, eAddAdToPage,
	eNofOptionsAddPageObjectToPage
} eAddPageObjectToPageMenuOptions;


static const char* eAddPageObjectToPageMenuOptionsStrings[eNofOptionsAddPageObjectToPage] = {"Add article", "Add ad"};

typedef enum
{
	eAddPageObjectToPage, eDeletePageObjectFromPage, eEditPageObjectInPage, 
	ePrintPage, eNofOptionsEditPage
} eEditPageMenuOptions;


static const char* eEditPageMenuOptionsStrings[eNofOptionsEditPage] = { "Add article or ad to page", "Delete one from the page", "Edit one in page", "Print the page"};

typedef struct {
	PageObject** pageObjects;
	int numOfPageObjects;
}Page;

void initPage(Page* page);
int addPageObjectToPage(Page* page, PageObjectStock* stock, char** newspaperTitles, int sizeOfTitles);
int findPageObjectIndexInPageByTitle(const Page* pPage, const char* title);
int deletePageObjectFromPage(Page* pPage);
int writePageToTextFile(const Page* pPage, FILE* fp, int toPublish);
int writePageToBinaryFile(const Page* pPage, FILE* fp);
int readPageFromTextFile(Page* pPage, FILE* fp);
int readPageFromBinaryFile(Page* pPage, FILE* fp);
PageObject* findObjectByTitleInPage(const Page* pPage, const char* titleToSearch);
int editPageObjectInPage(Page* pPage, PageObjectStock* stock, char** newspaperTitles, int sizeOfTitles);
int getPageIncomeOrOutcome(const Page* pPage);
void printPage(const Page* pPage);
void limitedPrintPage(const Page* pPage, int printListOfObjectsInPage);
void freePage(Page* pPage);