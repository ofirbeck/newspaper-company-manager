#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "PageObjectStock.h"
#include "General.h"


int initPageObjectStock(PageObjectStock* stock)
{
	return L_init(&stock->pageObjectList);
}

int getStockCount(const PageObjectStock* stock)
{
	NODE* ptr = stock->pageObjectList.head.next;
	int count = 0;
	while (ptr)
	{
		count++;
		ptr = ptr->next;
	}
	return count;
}

char** getTitleArrFromStock(const PageObjectStock* stock)
{
	char** titles = (char**)malloc(sizeof(char*) * getStockCount(stock));
	CHECK_RETURN_NULL(titles);
	NODE* ptr = stock->pageObjectList.head.next;
	int count = 0;
	PageObject* temp;
	while (ptr)
	{
		temp = (PageObject*)ptr->key;
		titles[count] = temp->title;
		count++;
		ptr = ptr->next;
	}
	return titles;
}

PageObject* findObjectByTitle(const PageObjectStock* stock, const char* titleToSearch)
{
	NODE* q = stock->pageObjectList.head.next;
	PageObject* temp;
	while (q)
	{
		temp = (PageObject*)q->key;
		if (strcmp(temp->title, titleToSearch) == 0)
			return temp;
		q = q->next;
	}
	return NULL;
}


int writeStockToTextFile(const PageObjectStock* pStock, const char* fileName)
{
	FILE* fp = fopen(fileName, "w+");
	CHECK_RETURN_ZERO(fp);

	int stockCount = getStockCount(pStock);
	if (fprintf(fp, "%d\n", stockCount) < 0)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	NODE* ptr = pStock->pageObjectList.head.next;
	PageObject* temp;
	for (int i = 0; i < stockCount; i++)
	{
		temp = (PageObject*)ptr->key;
		if (!temp->writeToTextFile(temp, fp, 0))
		{
			CLOSE_RETURN_ZERO(fp);
		}
		ptr = ptr->next;
	}
	fclose(fp);
	return  1;
}

int writeStockToBinaryFile(const PageObjectStock* pStock, const char* fileName)
{
	FILE* fp = fopen(fileName, "w+b");
	CHECK_RETURN_ZERO(fp);
	int stockCount = getStockCount(pStock);
	if (fwrite(&stockCount, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	NODE* ptr = pStock->pageObjectList.head.next;
	PageObject* temp;
	for (int i = 0; i < stockCount; i++)
	{
		temp = (PageObject*)ptr->key;
		if (!temp->writeToBinaryFile(temp, fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
		ptr = ptr->next;
	}
	fclose(fp);
	return  1;
}

int initPageObjectStockFromTextFile(PageObjectStock* stock, const char* fileName)
{
	if (!initPageObjectStock(stock))
		return 0;

	FILE* fp = fopen(fileName, "r");
	CHECK_RETURN_ZERO(fp);

	int stockCount;
	if (fscanf(fp, "%d", &stockCount) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	NODE* addingPlace = &stock->pageObjectList.head;
	char tempType[MAX_STR_LEN] = { 0 };
	for (int i = 0; i < stockCount; i++)
	{
		PageObject* pPageObject = (PageObject*)calloc(1, sizeof(PageObject));
		CHECK_CLOSE_RETURN_ZERO(pPageObject, fp);
		do {
			fMyGets(fp, tempType, MAX_STR_LEN);
		} while (strlen(tempType) < 2);
		strcat(tempType, "\n");
		char* type = _strdup(tempType);
		if (strcmp(type, START_OF_ARTICLE_IN_FILE) == 0)
			setArticleFuncs(pPageObject);
		else if (strcmp(type, START_OF_AD_IN_FILE) == 0)
			setAdFuncs(pPageObject);
		else
			return 0;

		if (!pPageObject->readFromTextFile(pPageObject, fp))
		{
			freeObjectStock(stock);
			CLOSE_RETURN_ZERO(fp);
		}
		L_insert(addingPlace, (void*)pPageObject);
		free(type);
	}
	fclose(fp);
	return 1;
}

int initPageObjectStockFromBinaryFile(PageObjectStock* stock, const char* fileName)
{
	if (!initPageObjectStock(stock))
		return 0;

	FILE* fp = fopen(fileName, "rb");
	CHECK_RETURN_ZERO(fp);
	int stockCount;
	if (fread(&stockCount, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	NODE* addingPlace = &stock->pageObjectList.head;
	int typeMessageLen;
	for (int i = 0; i < stockCount; i++)
	{
		PageObject* pPageObject = (PageObject*)calloc(1, sizeof(PageObject));
		CHECK_RETURN_ZERO(pPageObject);
		if (fread(&typeMessageLen, sizeof(int), 1, fp) != 1)
		{
			CLOSE_RETURN_ZERO(fp);
		}
		char* type = (char*)malloc(sizeof(char) * typeMessageLen);
		
		if (fread(type, sizeof(char), typeMessageLen, fp) != typeMessageLen)
		{
			CLOSE_RETURN_ZERO(fp);
		}
		if (strcmp(type, START_OF_AD_IN_FILE) == 0)
			setAdFuncs(pPageObject);
		else if (strcmp(type, START_OF_ARTICLE_IN_FILE) == 0)
			setArticleFuncs(pPageObject);
		else
			return 0;

		if (!pPageObject->readFromBinaryFile(pPageObject, fp))
		{
			freeObjectStock(stock);
			CLOSE_RETURN_ZERO(fp);
		}
		L_insert(addingPlace, (void*)pPageObject);
		free(type);
	}
	fclose(fp);
	return 1;
}


int editObjectInStock(PageObjectStock* stock)
{
	if (getStockCount(stock) == 0)
	{
		printf("There are no ads or articles in the stock at the moment, add some and come back\n");
		return 0;
	}
	limitedPrintStock(stock);
	char** stockTitles;
	PageObject* objectToEdit;
	while (1)
	{
		printf("Please write the title of the object you want to edit\n");
		char* title = getTitle();
		CHECK_RETURN_ZERO(title);
		objectToEdit = findObjectByTitle(stock, title);
		if (objectToEdit)
		{
			stockTitles = getTitleArrFromStock(stock);
			objectToEdit->edit(objectToEdit, stockTitles, getStockCount(stock));
			free(title);
			break;
		}
		free(title);
	}
	free(stockTitles);
	return 1;
}


int addObjectToStock(PageObjectStock* stock)
{
	PageObject* pPageObject = (PageObject*)calloc(1, sizeof(PageObject));
	if (!pPageObject)
		return 0;
	int option;
	while (1)
	{
		printf("Please choose if you want to create an article or an ad\n");
		printf("0 - for article\n");
		printf("1 - for ad\n");
		scanf("%d", &option);
		if (option == 0 || option == 1)
			break;
		printf("invalid option - please try again\n");
	}
	char** stockTitles = getTitleArrFromStock(stock);
	if (!stockTitles)
	{
		printf("error\n");
		return 0;
	}
	if (option == 0)
	{
		if (!initArticle(pPageObject, stockTitles, getStockCount(stock)))
			CHECK_FREE_RETURN_ZERO(pPageObject);
	}
	else if (option == 1)
	{
		if (!initAd(pPageObject, stockTitles, getStockCount(stock)))
			CHECK_FREE_RETURN_ZERO(pPageObject);
	}
	NODE* addingPlace = &stock->pageObjectList.head;
	L_insert(addingPlace, (void*)pPageObject);
	free(stockTitles);
	return 1;
}


// maybe we should keep ads that are requested to be removed, and remove only articles
int deleteObjectFromStock(PageObjectStock* stock, PageObject* toDelete)
{
	NODE* pNode = &(stock->pageObjectList.head);
	PageObject* temp;
	while (pNode->next)
	{
		temp = (PageObject*)pNode->next->key;
		if (strcmp(toDelete->title, temp->title) == 0)
		{
			if (!L_delete(pNode, temp->free))
				return 0;
			free(temp);
			return 1;
		}
		pNode = pNode->next;
	}
	return 0;
}


void limitedPrintStock(const PageObjectStock* stock)
{
	NODE* q = stock->pageObjectList.head.next;
	PageObject* temp;
	while (q)
	{
		temp = (PageObject*)q->key;
		temp->limitedPrint(temp);
		q = q->next;
	}
}

void freeObjectStock(PageObjectStock* stock)
{
	NODE* pNode = &(stock->pageObjectList.head);
	NODE* tmp;
	PageObject* pageObjectTemp;
	while (pNode->next)
	{
		tmp = pNode->next;
		pageObjectTemp = (PageObject*)tmp->key;
		pageObjectTemp->free(pageObjectTemp);
		free(pageObjectTemp);
		pNode->next = tmp->next;
		free(tmp);
	}
}