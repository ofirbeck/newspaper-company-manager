#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "PageObject.h"
#include "General.h"

char* getTitle()
{
	char tempTitle[MAX_TITLE_LENGTH];
	while (1)
	{
		printf("Please enter a title\n");
		myGets(tempTitle, MAX_TITLE_LENGTH);
		if (tempTitle)
			break;
	}
	char* title = getDynStr(tempTitle);
	if (!title)
		return NULL;
	return title;
}

char* getContent()
{
	char* content = NULL;
	char line[MAX_STR_LEN];
	size_t contentLen = 0;
	int isFirstLine = 1;
	printf("Please write below your content, line by line\n");
	printf("Enter 0 when you finished\n");

	while (contentLen <= MAX_PAGEOBJECT_CONTENT_LEN)
	{
		myGets(line, MAX_STR_LEN);
		if (strlen(line) == 1 && line[0] == '0')
			break;
		contentLen += (size_t)strlen(line) + 3;
		content = (char*)realloc(content, contentLen * sizeof(char));
		CHECK_RETURN_NULL(content);
		if (isFirstLine)
		{
			content[0] = '\0';
			isFirstLine = 0;
		}
		strcat(line, "\n");
		strcat(content, line);
	}
	return content;
}


void copyTitleAndContent(PageObject* destPageObject, PageObject* sourcePageObject)
{
	destPageObject->title = (char*)malloc(sizeof(char) * (strlen(sourcePageObject->title) + 1));
	if (!destPageObject->title)
		return;
	destPageObject->content = (char*)malloc(sizeof(char) * (strlen(sourcePageObject->content) + 1));
	if (!destPageObject->content)
		return;
	strcpy(destPageObject->title, sourcePageObject->title);
	strcpy(destPageObject->content, sourcePageObject->content);
}

int writePageObjectToTextFile(const PageObject* pPageObject, FILE* fp, int toPublish)
{
	if (toPublish)
		return (fprintf(fp, "			%s\n%s\n", pPageObject->title, pPageObject->content) >= 0);
	else
		return (fprintf(fp, "%s\n%zu\n%s\n", pPageObject->title, strlen(pPageObject->content), pPageObject->content) >= 0);
}

int writePageObjectToBinaryFile(const PageObject* pPageObject, FILE* fp)
{
	int titleLen = (int)(strlen(pPageObject->title) + 1);
	if (fwrite(&titleLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pPageObject->title, sizeof(char), titleLen, fp) != titleLen)
		return 0;
	int contentLen = (int)(strlen(pPageObject->content) + 1);
	if (fwrite(&contentLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pPageObject->content, sizeof(char), contentLen, fp) != contentLen)
		return 0;
	return 1;
}

int readPageObjectFromTextFile(PageObject* pPageObject, FILE* fp)
{
	char pageObjectTitleTemp[MAX_TITLE_LENGTH] = { 0 };
	char pageObjectContentLine[MAX_STR_LEN] = { 0 };
	char pageObjectContentTemp[MAX_PAGEOBJECT_CONTENT_LEN] = { 0 };
	do {
		fMyGets(fp, pageObjectTitleTemp, MAX_TITLE_LENGTH);
	} while (strlen(pageObjectTitleTemp) < 2);
	size_t lenOfContent;
	if (fscanf(fp, "%zu", &lenOfContent) != 1)
		return 0;
	do {
		do {
			fMyGets(fp, pageObjectContentLine, MAX_STR_LEN);
		} while ((strlen(pageObjectContentLine) < 2) && (!feof(fp)));

		if (strcmp(pageObjectContentLine, "") != 0)
		{
			strcat(pageObjectContentLine, "\n");
			strcat(pageObjectContentTemp, pageObjectContentLine);
			lenOfContent -= strlen(pageObjectContentLine);
		}
	} while (!feof(fp) && lenOfContent > 0);

	pPageObject->title = _strdup(pageObjectTitleTemp);
	pPageObject->content = _strdup(pageObjectContentTemp);
	if (!pPageObject->title || !pPageObject->content)
		return 0;
	return 1;
}

int readPageObjectFromBinaryFile(PageObject* pPageObject, FILE* fp)
{
	int titleLen = 0;
	if (fread(&titleLen, sizeof(int), 1, fp) != 1)
		return 0;

	pPageObject->title = (char*)malloc(titleLen * sizeof(char));
	CHECK_RETURN_ZERO(pPageObject->title);
	if (fread(pPageObject->title, sizeof(char), titleLen, fp) != titleLen)
	{
		free(pPageObject->title);
		return 0;
	}
	int contentLen = 0;
	if (fread(&contentLen, sizeof(int), 1, fp) != 1)
		return 0;
	pPageObject->content = (char*)malloc(contentLen * sizeof(char));
	CHECK_RETURN_ZERO(pPageObject->content);
	if (fread(pPageObject->content, sizeof(char), contentLen, fp) != contentLen)
	{
		free(pPageObject->content);
		return 0;
	}
	return 1;
}

char* addLinesToContent(char* oldContent)
{
	char* newContent = NULL;
	char newLine[MAX_STR_LEN];
	size_t contentLen = 0;
	int addToEnd = -1;
	int isFirstLine = 1;
	while (1)
	{
		printf("Please choose where to add new lines\n");
		printf("0 - to the start of the content\n");
		printf("1 - to the end of the content\n");
		scanf("%d", &addToEnd);
		if (addToEnd == 0 || addToEnd == 1)
			break;
	}
	if (addToEnd && oldContent)
	{
		contentLen = strlen(oldContent);
		newContent = (char*)malloc(contentLen * sizeof(char) + 1);
		CHECK_RETURN_NULL(newContent);
		strcpy(newContent, oldContent);
		isFirstLine = 0;
	}
	printf("Please write below the lines you want to add to the content, line by line\n");
	printf("Enter 0 when you finished\n");
	while (contentLen <= MAX_PAGEOBJECT_CONTENT_LEN)
	{
		myGets(newLine, MAX_STR_LEN);
		if (strlen(newLine) == 1 && newLine[0] == '0')
			break;
		contentLen += (size_t)strlen(newLine) + 3;
		newContent = (char*)realloc(newContent, contentLen * sizeof(char));
		CHECK_RETURN_NULL(newContent);
		if (isFirstLine)
		{
			newContent[0] = '\0';
			isFirstLine = 0;
		}
		strcat(newLine, "\n");
		strcat(newContent, newLine);
	}
	if (!addToEnd)
	{
		contentLen += (size_t)strlen(oldContent);
		newContent = (char*)realloc(newContent, contentLen * sizeof(char));
		CHECK_RETURN_NULL(newContent);
		strcat(newContent, oldContent);
	}
	return newContent;
}

int isTitleInTitleArr(const char** titles, int sizeOfTitles, const char* titleToSearch)
{
	for (int i = 0; i < sizeOfTitles; i++)
	{
		if (strcmp(titles[i], titleToSearch) == 0)
			return 1;
	}
	return 0;
}


void editPageObject(PageObject* pPageObject, char** titles, int sizeOfTitles, int option)
{
	switch (option)
	{
	case eChangetitle:
		while (1)
		{
			char* newTitle = getTitle();
			if (!newTitle)
			{
				printf("Error editing page object\n");
				return;
			}
			if (!isTitleInTitleArr(titles, sizeOfTitles, newTitle))
			{
				free(pPageObject->title);
				pPageObject->title = newTitle;
				break;
			}
			else
			{
				free(newTitle);
				printf("This title is already in use, please enter another one\n");
			}
		}
		printf("The title has been changed successfully \n");
		break;
	case eChangeContent:
		free(pPageObject->content);
		pPageObject->content = getContent();
		printf("The content has been changed successfully \n");
		break;
	case eAddLines:
		pPageObject->content = addLinesToContent(pPageObject->content);
		printf("The new content is: \n%s", pPageObject->content);
		break;
	}
}

void limitedPrintPageObjectPtr(void* pPageObjectPtr)
{
	PageObject* temp = *(PageObject**)pPageObjectPtr;
	temp->limitedPrint(temp);
}


void printPageObject(const PageObject* pPageObject)
{
	printf("The title is: %s\n", pPageObject->title);
	printf("The content is: \n%s", pPageObject->content);
}

void freePageObjectPtr(void* pPageObjectPtr)
{
	PageObject* temp = *(PageObject**)pPageObjectPtr;
	temp->free(temp);
}

void freePageObject(PageObject* pPageObject)
{
	free(pPageObject->title);
	free(pPageObject->content);
}