#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "PageObjectStock.h"
#include "Page.h"
#include "General.h"


void initPage(Page* page)
{
	page->pageObjects = NULL;
	page->numOfPageObjects = 0;
}

int addPageObjectToPage(Page* page, PageObjectStock* stock, char** newspaperTitles, int sizeOfTitles)
{
	if (page->numOfPageObjects == MAX_PAGEOBJECTS_PER_PAGE)
	{
		printf("Passed the max objects allowed in the page, exitting adding mode\n");
		return 0;
	}
	if (getStockCount(stock) == 0)
	{
		printf("Out of ads and articles to add to the page from the stock, please add some and come back\n");
		return 0;
	}
	printf("Here is the stock, you can choose from it:\n");
	limitedPrintStock(stock);
	PageObject* needsToBeCopied;
	int run = 1;
	while (run)
	{
		if (page->numOfPageObjects == MAX_PAGEOBJECTS_PER_PAGE)
		{
			printf("Passed the max objects allowed in the page, exitting adding mode\n");
			break;
		}
		printf("Please choose which of these you want to add to the page\n");
		char* title = getTitle();
		CHECK_RETURN_ZERO(title);
		
		needsToBeCopied = findObjectByTitle(stock, title);
		if (!needsToBeCopied)
		{
			printf("Such an ad or article with this title doesn't exsits in the stock, please try again\n");
			free(title);
			continue;
		}
		if (isTitleInTitleArr(newspaperTitles, sizeOfTitles, title))
		{
			printf("This title is already in the newspaper, please choose a different title\n");
			free(title);
			continue;
		}
		free(title);

		PageObject* objectToAdd = (PageObject*)calloc(1, sizeof(PageObject));
		CHECK_RETURN_ZERO(objectToAdd);
		needsToBeCopied->copy(objectToAdd, needsToBeCopied);
		if (objectToAdd)
		{
			page->pageObjects = (PageObject**)realloc(page->pageObjects, sizeof(PageObject*) * (page->numOfPageObjects + 1));
			CHECK_RETURN_ZERO(page->pageObjects);
			page->pageObjects[page->numOfPageObjects] = objectToAdd;
			if (!deleteObjectFromStock(stock, objectToAdd))
				return 0;
			page->numOfPageObjects++;
			printf("\nWhat you chose has been added successfully\n");
			if (getStockCount(stock) != 0)
			{
				while (1)
				{
					printf("Do you want to keep adding objects to this page?\n");
					printf("Type 0 to exit, 1 to keep adding\n");
					scanf("%d", &run);
					if (run == 0 || run == 1)
						break;
				}
				printf("\n");
			}
			else
			{
				printf("Out of ads and articles to add to the page from the stock, please add some and come back\n");
				run = 0;
			}
		}
		else
			printf("Invalid title, please choose one from the list above\n");
	}
	return 1;
}

int findPageObjectIndexInPageByTitle(const Page* pPage, const char* title)
{
	for (int i = 0; i < pPage->numOfPageObjects; i++)
	{
		if (strcmp(pPage->pageObjects[i]->title, title) == 0)
			return i;
	}
	return -1;
}

int deletePageObjectFromPage(Page* pPage)
{
	if (pPage->numOfPageObjects == 0)
	{
		printf("There is nothing to delete in this page\n");
		return 0;
	}
	limitedPrintPage(pPage, 1);
	int indexToDelete;
	while (1)
	{
		printf("Please choose which one of them you wish to delete entirely\n");
		char* titleToDelete = getTitle();
		CHECK_RETURN_ZERO(titleToDelete);
		indexToDelete = findPageObjectIndexInPageByTitle(pPage, titleToDelete);
		if (indexToDelete != -1)
		{
			free(titleToDelete);
			break;
		}
		else
			printf("Invalid title, try again\n");
		free(titleToDelete);
	}
	// swaps the deleted object with the last one, frees it and decrese the total num of objects
	PageObject* temp = pPage->pageObjects[indexToDelete];
	pPage->pageObjects[indexToDelete] = pPage->pageObjects[pPage->numOfPageObjects-1];
	pPage->pageObjects[pPage->numOfPageObjects - 1] = temp;
	pPage->pageObjects[pPage->numOfPageObjects - 1]->free(pPage->pageObjects[pPage->numOfPageObjects - 1]);
	free(pPage->pageObjects[pPage->numOfPageObjects - 1]);
	pPage->numOfPageObjects--;
	return 1;
}

int writePageToTextFile(const Page* pPage, FILE* fp, int toPublish)
{
	if (!toPublish)
		if (fprintf(fp, "%d\n", pPage->numOfPageObjects) < 0)
			return 0;
	if(pPage->pageObjects == 0 && toPublish)
		if (fprintf(fp, "Empty Page\n") < 0)
			return 0;
	for (int i = 0; i < pPage->numOfPageObjects; i++)
	{
		if (!pPage->pageObjects[i]->writeToTextFile(pPage->pageObjects[i], fp, toPublish))
			return 0;
	}
	return 1;
}

int writePageToBinaryFile(const Page* pPage, FILE* fp)
{
	if (fwrite(&pPage->numOfPageObjects, sizeof(int), 1, fp) != 1)
		return 0;
	for (int i = 0; i < pPage->numOfPageObjects; i++)
	{
		if (!pPage->pageObjects[i]->writeToBinaryFile(pPage->pageObjects[i], fp))
			return 0;
	}
	return 1;
}

int readPageFromTextFile(Page* pPage, FILE* fp)
{
	if (fscanf(fp, "%d", &pPage->numOfPageObjects) != 1)
		return 0;
	if (pPage->numOfPageObjects != 0)
	{
		pPage->pageObjects = (PageObject**)malloc(pPage->numOfPageObjects * sizeof(PageObject*));
		CHECK_RETURN_ZERO(pPage->pageObjects);
	}
	else
	{
		pPage->pageObjects = NULL;
		return 1;
	}
	if (!pPage->pageObjects)
		return 0;
	char tempType[MAX_STR_LEN] = { 0 };
	for (int i = 0; i < pPage->numOfPageObjects; i++)
	{
		pPage->pageObjects[i] = (PageObject*)calloc(1, sizeof(PageObject));
		CHECK_RETURN_ZERO(pPage->pageObjects[i]);
		do {
			fMyGets(fp, tempType, MAX_STR_LEN);
		} while (strlen(tempType) < 2);
		strcat(tempType, "\n");
		char* type = _strdup(tempType);
		if (strcmp(type, START_OF_ARTICLE_IN_FILE) == 0)
			setArticleFuncs(pPage->pageObjects[i]);
		else if (strcmp(type, START_OF_AD_IN_FILE) == 0)
			setAdFuncs(pPage->pageObjects[i]);
		else
			return 0;
		if (!pPage->pageObjects[i]->readFromTextFile(pPage->pageObjects[i], fp))
			return 0;
		free(type);
	}
	return 1;
}

int readPageFromBinaryFile(Page* pPage, FILE* fp)
{
	if (fread(&pPage->numOfPageObjects, sizeof(int), 1, fp) != 1)
		return 0;
	if (pPage->numOfPageObjects != 0)
	{
		pPage->pageObjects = (PageObject**)malloc(pPage->numOfPageObjects * sizeof(PageObject*));
		CHECK_RETURN_ZERO(pPage->pageObjects);
	}
	else
	{
		pPage->pageObjects = NULL;
		return 1;
	}
	int messageLen;
	for (int i = 0; i < pPage->numOfPageObjects; i++)
	{
		pPage->pageObjects[i] = (PageObject*)calloc(1, sizeof(PageObject));
		CHECK_RETURN_ZERO(pPage->pageObjects[i]);
		if (fread(&messageLen, sizeof(int), 1, fp) != 1)
			return 0;
		char* type = (char*)malloc(sizeof(char) * (messageLen));
		CHECK_RETURN_ZERO(type);
		if (fread(type, sizeof(char), messageLen, fp) != messageLen)
			return 0;
		if (strcmp(type, START_OF_AD_IN_FILE) == 0)
			setAdFuncs(pPage->pageObjects[i]);
		else if (strcmp(type, START_OF_ARTICLE_IN_FILE) == 0)
			setArticleFuncs(pPage->pageObjects[i]);
		else
			return 0;
		pPage->pageObjects[i]->readFromBinaryFile(pPage->pageObjects[i], fp);
		free(type);
	}
	return 1;
}

PageObject* findObjectByTitleInPage(const Page* pPage, const char* titleToSearch)
{
	for (int i = 0; i < pPage->numOfPageObjects; i++)
	{
		if (strcmp(pPage->pageObjects[i]->title, titleToSearch) == 0)
			return pPage->pageObjects[i];
	}
	return NULL;
}

int editPageObjectInPage(Page* pPage, PageObjectStock* stock, char** newspaperTitles, int sizeOfTitles)
{
	if (pPage->numOfPageObjects == 0)
	{
		printf("There is nothing to edit in this page, add an ad or an article and come back\n");
		return 0;
	}
	limitedPrintPage(pPage, 1);
	PageObject* objectToEdit;
	while (1)
	{
		printf("Please choose which one of them you want to edit\n");
		char* title = getTitle();
		CHECK_RETURN_ZERO(title);
		objectToEdit = findObjectByTitleInPage(pPage, title);
		if (objectToEdit)
		{
			objectToEdit->edit(objectToEdit, newspaperTitles, sizeOfTitles);
			free(title);
			break;
		}
		free(title);
	}
	return 1;
}


int getPageIncomeOrOutcome(const Page* pPage)
{
	int sum = 0;
	for (int i = 0; i < pPage->numOfPageObjects; i++)
	{
		sum += pPage->pageObjects[i]->getIncomeOrOutcome(pPage->pageObjects[i]);
	}
	return sum;
}

void printPage(const Page* pPage)
{
	for (int i = 0; i < pPage->numOfPageObjects; i++)
		pPage->pageObjects[i]->print(pPage->pageObjects[i]);
}

void limitedPrintPage(const Page* pPage, int printListOfObjectsInPage)
{
	printf("There are %d articles and ads in this page\n", pPage->numOfPageObjects);
	if (pPage->numOfPageObjects != 0 && printListOfObjectsInPage)
	{
		printf("Here is the list of them\n");
		generalArrayFunction(pPage->pageObjects, pPage->numOfPageObjects, sizeof(PageObject*), limitedPrintPageObjectPtr);
	}
}

void freePage(Page* pPage)
{
	generalArrayFunction(pPage->pageObjects, pPage->numOfPageObjects, sizeof(PageObject*), freePageObjectPtr);

	for (int i = 0; i < pPage->numOfPageObjects; i++)
		free(pPage->pageObjects[i]);

	if(pPage->pageObjects)
		free(pPage->pageObjects);
}