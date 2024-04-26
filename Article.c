#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "PageObject.h"


void setArticleFuncs(PageObject* pPageObject)
{
	pPageObject->print = printArticle;
	pPageObject->limitedPrint = limitedPrintArticle;
	pPageObject->writeToTextFile = writeArticleToTextFile;
	pPageObject->writeToBinaryFile = writeArticleToBinaryFile;
	pPageObject->readFromTextFile = readArticleFromTextFile;
	pPageObject->readFromBinaryFile = readArticleFromBinaryFile;
	pPageObject->copy = copyArticles;
	pPageObject->getIncomeOrOutcome = getArticleOutcome;
	pPageObject->edit = editArticle;
	pPageObject->free = freeArticle;
}

int getArticleOutcome(const PageObject* pPageObject)
{
	return -1 * pPageObject->type.article.artWriter.paymentForArticle;
}

int initArticle(PageObject* pPageObject, char** titles, int sizeOfTitles)
{
	setArticleFuncs(pPageObject);
	initWriter(&pPageObject->type.article.artWriter);
	while (1)
	{
		pPageObject->title = getTitle();
		CHECK_RETURN_ZERO(pPageObject->title);
		if (!isTitleInTitleArr(titles, sizeOfTitles, pPageObject->title))
			break;
		else
		{
			printf("This title is already in use, please enter another one\n");
			free(pPageObject->title);
		}
	}
	pPageObject->content = getContent();
	if (!pPageObject->content || !pPageObject->title)
		return 0;
	return 1;
}

void copyArticles(PageObject* destPageObject, PageObject* sourcePageObject)
{
	setArticleFuncs(destPageObject);
	copyWriters(&destPageObject->type.article.artWriter, &sourcePageObject->type.article.artWriter);
	copyTitleAndContent(destPageObject, sourcePageObject);
}


int writeArticleToTextFile(const PageObject* pPageObject, FILE* fp, int toPublish)
{
	if (!toPublish)
		if (fprintf(fp, "%s", START_OF_ARTICLE_IN_FILE) < 0)
			return 0;
	if (!writeWriterToTextFile(&pPageObject->type.article.artWriter, fp, toPublish))
		return 0;
	return writePageObjectToTextFile(pPageObject, fp, toPublish);
}

int writeArticleToBinaryFile(const PageObject* pPageObject, FILE* fp)
{
	int messageLen = (int)(strlen(START_OF_ARTICLE_IN_FILE) + 1);
	if (fwrite(&messageLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(START_OF_ARTICLE_IN_FILE, sizeof(char), messageLen, fp) != messageLen)
		return 0;
	if (!writeWriterToBinaryFile(&pPageObject->type.article.artWriter, fp))
		return 0;
	return writePageObjectToBinaryFile(pPageObject, fp);
}

int readArticleFromTextFile(PageObject* pPageObject, FILE* fp)
{
	if (!readWriterFromTextFile(&pPageObject->type.article.artWriter, fp))
		return 0;
	return readPageObjectFromTextFile(pPageObject, fp);
}

int readArticleFromBinaryFile(PageObject* pPageObject, FILE* fp)
{
	if (!readWriterFromBinaryFile(&pPageObject->type.article.artWriter, fp))
		return 0;
	return readPageObjectFromBinaryFile(pPageObject, fp);
}

int editArticleOptionsMenu()
{
	const char* editArticleOptions[eNofOptionsEditPageObject + eNofOptionsEditArticle];
	for (int i = 0; i < eNofOptionsEditPageObject; i++)
		editArticleOptions[i] = eEditPageObjectMenuOptionsStrings[i];
	for (int i = eNofOptionsEditPageObject; i < eNofOptionsEditArticle + eNofOptionsEditPageObject; i++)
		editArticleOptions[i] = eUniqueEditArticleMenuOptionsStrings[i - eNofOptionsEditPageObject];
	return menu(editArticleOptions, eNofOptionsEditPageObject + eNofOptionsEditArticle);
}


void editArticle(PageObject* pPageObject, char** titles, int sizeOfTitles)
{
	int option;
	while (1)
	{
		option = editArticleOptionsMenu();
		if (option == EXIT)
		{
			printf("Exitting edit mode\n");
			break;
		}
		if (option >= 0 && option <= eNofOptionsEditPageObject - 1)
			editPageObject(pPageObject, titles, sizeOfTitles, option);
		else
		{
			option -= eNofOptionsEditPageObject;
			switch (option)
			{
			case eChangeArticleWriter:
				freeWriter(&pPageObject->type.article.artWriter);
				initWriter(&pPageObject->type.article.artWriter);
				printf("The writer has been changed successfully");
				break;
			case ePrintArticle:
				pPageObject->print(pPageObject);
				printf("\n");
				break;
			default:
				printf("Wrong option\n");
				break;
			}
		}
	}
}

void printArticle(const PageObject* pPageObject)
{
	printf("The Writer of the article is: \n");
	printWriter(&pPageObject->type.article.artWriter);
	printPageObject(pPageObject);
}

void limitedPrintArticle(const PageObject* pPageObject)
{
	printf("Article named: %s, its writer is: %s\n", pPageObject->title, pPageObject->type.article.artWriter.name);
}

void freeArticle(PageObject* pPageObject)
{
	freePageObject(pPageObject);
	freeWriter(&pPageObject->type.article.artWriter);
}

