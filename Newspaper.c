#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Newspaper.h"
#include "General.h"

int initNewspaper(Newspaper* pNewspaper, char uniqueCode[EDITION_CODE_LENGTH + 1])
{
	pNewspaper->allPages = NULL;
	pNewspaper->pageCount = 0;
	strcpy(pNewspaper->editionCode, uniqueCode);
	return addPages(pNewspaper, 1);
}

void getEditionCode(char* code)
{
	char temp[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter edition code  - %d letters\n", EDITION_CODE_LENGTH);
		myGets(temp, MAX_STR_LEN);
		if (strlen(temp) != EDITION_CODE_LENGTH)
		{
			printf("code should be %d letters\n", EDITION_CODE_LENGTH);
			ok = 0;
		}
		else {
			for (int i = 0; i < EDITION_CODE_LENGTH; i++)
			{
				if (isalpha(temp[i]) == 0)
				{
					printf("Needs to be letter\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);
	strcpy(code, temp);
}

int addPages(Newspaper* pNewspaper, int isNewNewspaper)
{
	if (pNewspaper->pageCount == MAX_PAGES_IN_NEWSPAPER)
	{
		printf("You can't add more pages to this newspaper\n");
		return 0;
	}
	int amount = 0;
	while (1)
	{
		if(!isNewNewspaper)
			printf("You have %d pages in the newspaper, how much you want to add?\n", pNewspaper->pageCount);
		else
		{
			printf("Please enter the amount of pages you want in the newspaper\n");
			printf("You could always add more pages in the future\n");
		}
		printf("Please note that the newspaper should not contain more than %d pages\n", MAX_PAGES_IN_NEWSPAPER);
		scanf("%d", &amount);
		if (amount > 0)
		{
			if (amount + pNewspaper->pageCount <= MAX_PAGES_IN_NEWSPAPER)
			{
				break;
			}
			else
				printf("The total amount exceeded the maximum pages possible\n");
		}
		else
			printf("Can't add a negative amount of pages\n");
	}
	for (int i = 0; i < amount; i++)
	{
		Page* pPage = (Page*)calloc(1, sizeof(Page));
		CHECK_RETURN_ZERO(pPage);
		initPage(pPage);
		pNewspaper->allPages = (Page**)realloc(pNewspaper->allPages, sizeof(Page*) * (pNewspaper->pageCount + 1));
		CHECK_RETURN_ZERO(pNewspaper->allPages);
		pNewspaper->allPages[pNewspaper->pageCount] = pPage;
		pNewspaper->pageCount++;
	}
	if(!isNewNewspaper)
		printf("The new pages were added successfully, you can choose to edit them now\n");
	return 1;
}

int getNumOfTitles(const Newspaper* pNewspaper)
{
	int count = 0;
	for (int i = 0; i < pNewspaper->pageCount; i++)
		count += pNewspaper->allPages[i]->numOfPageObjects;
	return count;
}

void updateTitleArrFromNewspaper(const Newspaper* pNewspaper, char*** titles, int* countOfTitles)
{
	int count = 0;
	for (int i = 0; i < pNewspaper->pageCount; i++)
	{
		for (int j = 0; j < pNewspaper->allPages[i]->numOfPageObjects; j++)
		{
			(*titles)[count] = pNewspaper->allPages[i]->pageObjects[j]->title;
			count++;
		}
	}
	*countOfTitles = count;
}


int editPage(Page* pPage, PageObjectStock* stock, Newspaper* pNewspaper)
{
	int stop = 0;
	int option;
	char** newspaperTitles = (char**)malloc(sizeof(char*) * MAX_PAGES_IN_NEWSPAPER * MAX_PAGEOBJECTS_PER_PAGE);
	CHECK_RETURN_ZERO(newspaperTitles);
	int sizeOfTitles = 0;
	while (!stop)
	{
		updateTitleArrFromNewspaper(pNewspaper, &newspaperTitles, &sizeOfTitles);
		option = menu(eEditPageMenuOptionsStrings, (int)eNofOptionsEditPage);
		switch (option)
		{
		case eAddPageObjectToPage:
			if (!addPageObjectToPage(pPage, stock, newspaperTitles, sizeOfTitles))
				printf("Error adding article or ad to page\n");
			break;
		case eDeletePageObjectFromPage:
			if (!deletePageObjectFromPage(pPage))
				printf("Error deleting article or ad from page\n");
			break;
		case eEditPageObjectInPage:
			if (!editPageObjectInPage(pPage, stock, newspaperTitles, sizeOfTitles))
				printf("Error editting article or ad in page\n");
			break;
		case ePrintPage:
			printPage(pPage);
			break;
		case EXIT:
			stop = 1;
			break;
		default:
			printf("Wrong option\n");
			break;
		}
	}
	free(newspaperTitles);
	return 1;
}

int editPagesInNewspaper(Newspaper* pNewspaper, PageObjectStock* stock)
{
	int editNewspaper = 1;
	while (editNewspaper)
	{
		limitedPrintNewspaper(pNewspaper);
		int editPageNum = 0;
		while (1)
		{
			printf("Choose which of the pages above you want to edit, enter its number\n");
			scanf("%d", &editPageNum);
			if (editPageNum - 1 >= 0 && editPageNum - 1 < pNewspaper->pageCount)
				break;
		}
		Page* requestedPage = pNewspaper->allPages[editPageNum - 1];
		if (!editPage(requestedPage, stock, pNewspaper))
			return 0;
		while (1)
		{
			printf("Do you wish to keep editing this newspaper?\n");
			printf("Choose 1 to keep editing, 0 to stop editing\n");
			scanf("%d", &editNewspaper);
			if (editNewspaper == 0 || editNewspaper == 1)
				break;
		}
	}
	return 1;
}

int writeNewspaperToTextFile(const Newspaper* pNewspaper, FILE* fp, int toPublish)
{
	if (!toPublish)
	{
		if (fprintf(fp, "%s\n", pNewspaper->editionCode) < 0)
			return 0;
		if (fprintf(fp, "%d\n", pNewspaper->pageCount) < 0)
			return 0;
	}
	else
	{
		if (fprintf(fp, "The newspaper edition: %s\n", pNewspaper->editionCode) < 0)
			return 0;
	}

	if(toPublish)
		if (fprintf(fp, "-----------------------------------------------------------\n") < 0)
			return 0;
		
	for (int i = 0; i < pNewspaper->pageCount; i++)
	{
		if (!writePageToTextFile(pNewspaper->allPages[i], fp, toPublish))
			return 0;
		if (toPublish)
		{
			if (fprintf(fp, "                         -%d-                              \n", i + 1) < 0)
				return 0;
			if (fprintf(fp, "-----------------------------------------------------------\n") < 0)
				return 0;
		}
	}
	return 1;
}

int writeNewspaperToBinaryFile(const Newspaper* pNewspaper, FILE* fp)
{
	if (fwrite(pNewspaper->editionCode, sizeof(char), EDITION_CODE_LENGTH + 1, fp) != EDITION_CODE_LENGTH + 1)
		return 0;
	if (fwrite(&pNewspaper->pageCount, sizeof(int), 1, fp) != 1)
		return 0;
	for (int i = 0; i < pNewspaper->pageCount; i++)
	{
		if (!writePageToBinaryFile(pNewspaper->allPages[i], fp))
			return 0;
	}
	return 1;
}

int readNewspaperFromTextFile(Newspaper* pNewspaper, FILE* fp)
{
	if (fscanf(fp, "%s", &pNewspaper->editionCode) != 1)
		return 0;
	if (fscanf(fp, "%d", &pNewspaper->pageCount) != 1)
		return 0;
	pNewspaper->allPages = (Page**)malloc(pNewspaper->pageCount * sizeof(Page*));
	CHECK_RETURN_ZERO(pNewspaper->allPages);
	for (int i = 0; i < pNewspaper->pageCount; i++)
	{
		pNewspaper->allPages[i] = (Page*)calloc(1, sizeof(Page));
		CHECK_RETURN_ZERO(pNewspaper->allPages[i]);
		if (!readPageFromTextFile(pNewspaper->allPages[i], fp))
			return 0;
	}
	return 1;
}

int readNewspaperFromBinaryFile(Newspaper* pNewspaper, FILE* fp)
{
	if (fread(pNewspaper->editionCode, sizeof(char), EDITION_CODE_LENGTH + 1, fp) != EDITION_CODE_LENGTH + 1)
		return 0;
	if (fread(&pNewspaper->pageCount, sizeof(int), 1, fp) != 1)
		return 0;
	pNewspaper->allPages = (Page**)malloc(pNewspaper->pageCount * sizeof(Page*));
	CHECK_RETURN_ZERO(pNewspaper->allPages);
	for (int i = 0; i < pNewspaper->pageCount; i++)
	{
		pNewspaper->allPages[i] = (Page*)calloc(1, sizeof(Page));
		CHECK_RETURN_ZERO(pNewspaper->allPages[i]);
		if (!readPageFromBinaryFile(pNewspaper->allPages[i], fp))
			return 0;
	}
	return 1;
}

int getNewspaperIncomeOrOutcome(const Newspaper* pNewspaper)
{
	int sum = 0;
	for (int i = 0; i < pNewspaper->pageCount; i++)
		sum += getPageIncomeOrOutcome(pNewspaper->allPages[i]);
	return sum;
}


void limitedPrintNewspaper(const Newspaper* pNewspaper)
{
	printf("There are %d pages in this newspaper that its code is %s\n", pNewspaper->pageCount, pNewspaper->editionCode);
	for(int i = 0; i < pNewspaper->pageCount; i++)
	{
		printf("%d.", i + 1);
		limitedPrintPage(pNewspaper->allPages[i], 0);
	}
}

void freeNewspaper(Newspaper* pNewspaper)
{
	for (int i = 0; i < pNewspaper->pageCount; i++)
	{
		freePage(pNewspaper->allPages[i]);
		free(pNewspaper->allPages[i]);
	}
	if(pNewspaper->allPages)
		free(pNewspaper->allPages);
}

