#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "PageObject.h"


int getAdIncome(const PageObject* pPageObject)
{
	return pPageObject->type.ad.advertisingPrice;
}

char* getAdvertiserName()
{
	char* advertiserName;
	while (1)
	{
		advertiserName = getStrExactName("Enter advertiser's name\n");
		if (advertiserName)
			break;
	}
	return advertiserName;
}


int getAdvertisingPrice()
{
	int advertisingPrice;
	while (1)
	{
		printf("Enter the advertising price for this Ad\n");
		printf("Please note it should not be under 0\n");
		scanf("%d", &advertisingPrice);
		if (CHECK_VALID_PRICE(advertisingPrice))
			break;
		else
			printf("Invalid price, please try again\n");
	}
	return advertisingPrice;
}

void setAdFuncs(PageObject* pPageObject)
{
	pPageObject->print = printAd;
	pPageObject->limitedPrint = limitedPrintAd;
	pPageObject->writeToTextFile = writeAdToTextFile;
	pPageObject->writeToBinaryFile = writeAdToBinaryFile;
	pPageObject->readFromTextFile = readAdFromTextFile;
	pPageObject->readFromBinaryFile = readAdFromBinaryFile;
	pPageObject->copy = copyAds;
	pPageObject->getIncomeOrOutcome = getAdIncome;
	pPageObject->edit = editAd;
	pPageObject->free = freeAd;
}


int initAd(PageObject* pPageObject, char** titles, int sizeOfTitles)
{
	setAdFuncs(pPageObject);
	pPageObject->type.ad.advertiserName = getAdvertiserName();
	pPageObject->type.ad.advertisingPrice = getAdvertisingPrice();
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


void copyAds(PageObject* destPageObject, PageObject* sourcePageObject)
{
	setAdFuncs(destPageObject);
	destPageObject->type.ad.advertiserName = (char*)malloc(sizeof(char) * (strlen(sourcePageObject->type.ad.advertiserName) + 1));
	if (!destPageObject->type.ad.advertiserName)
		return;
	strcpy(destPageObject->type.ad.advertiserName, sourcePageObject->type.ad.advertiserName);
	destPageObject->type.ad.advertisingPrice = sourcePageObject->type.ad.advertisingPrice;
	copyTitleAndContent(destPageObject, sourcePageObject);
}

int writeAdToTextFile(const PageObject* pPageObject, FILE* fp, int toPublish)
{
	if(!toPublish)
		if (fprintf(fp, "%s", START_OF_AD_IN_FILE) < 0)
			return 0;
	if (toPublish)
	{
		if (fprintf(fp, "Brought to you by: %s\n", pPageObject->type.ad.advertiserName) < 0)
			return 0;
	}
	else
	{
		if (fprintf(fp, "%s\n%d\n", pPageObject->type.ad.advertiserName, pPageObject->type.ad.advertisingPrice) < 0)
			return 0;
	}	
	return writePageObjectToTextFile(pPageObject, fp, toPublish);
}

int writeAdToBinaryFile(const PageObject* pPageObject, FILE* fp)
{
	int messageLen = (int)(strlen(START_OF_AD_IN_FILE) + 1);
	if (fwrite(&messageLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(START_OF_AD_IN_FILE, sizeof(char), messageLen, fp) != messageLen)
		return 0;
	int NameLen = (int)(strlen(pPageObject->type.ad.advertiserName) + 1);
	if (fwrite(&NameLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pPageObject->type.ad.advertiserName, sizeof(char), NameLen, fp) != NameLen)
		return 0;
	if (fwrite(&pPageObject->type.ad.advertisingPrice, sizeof(int), 1, fp) != 1)
		return 0;
	return writePageObjectToBinaryFile(pPageObject, fp);
}


int readAdFromTextFile(PageObject* pPageObject, FILE* fp)
{
	char advertiserNameTemp[MAX_STR_LEN] = { 0 };
	do {
		fMyGets(fp, advertiserNameTemp, MAX_STR_LEN);
	} while (strlen(advertiserNameTemp) < 2);

	pPageObject->type.ad.advertiserName = _strdup(advertiserNameTemp);

	if (!pPageObject->type.ad.advertiserName)
		return 0;
	if (fscanf(fp, "%d", &pPageObject->type.ad.advertisingPrice) != 1)
		return 0;
	return readPageObjectFromTextFile(pPageObject, fp);
}

int readAdFromBinaryFile(PageObject* pPageObject, FILE* fp)
{
	int nameLen = 0;
	if (fread(&nameLen, sizeof(int), 1, fp) != 1)
		return 0;

	pPageObject->type.ad.advertiserName = (char*)malloc(nameLen * sizeof(char));
	CHECK_RETURN_ZERO(pPageObject->type.ad.advertiserName);
	if (fread(pPageObject->type.ad.advertiserName, sizeof(char), nameLen, fp) != nameLen)
	{
		free(pPageObject->type.ad.advertiserName);
		return 0;
	}
	if (fread(&pPageObject->type.ad.advertisingPrice, sizeof(int), 1, fp) != 1)
		return 0;
	return readPageObjectFromBinaryFile(pPageObject, fp);
}
int editAdOptionsMenu()
{
	const char* editAdOptions[eNofOptionsEditPageObject + eNofOptionsEditAd];
	for (int i = 0; i < eNofOptionsEditPageObject; i++)
		editAdOptions[i] = eEditPageObjectMenuOptionsStrings[i];
	for (int i = eNofOptionsEditPageObject; i < eNofOptionsEditAd + eNofOptionsEditPageObject; i++)
		editAdOptions[i] = eUniqueEditAdMenuOptionsStrings[i - eNofOptionsEditPageObject];
	return menu(editAdOptions, eNofOptionsEditPageObject + eNofOptionsEditAd);
}


void editAd(PageObject* pPageObject, char** titles, int sizeOfTitles)
{
	int option;
	while (1)
	{
		option = editAdOptionsMenu();
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
			case eChangeAdAdvertiserName:
				free(pPageObject->type.ad.advertiserName);
				pPageObject->type.ad.advertiserName = getAdvertiserName();
				printf("The advertiser name has been changed successfully");
				break;
			case eChangeAdAdvertisingPrice:
				pPageObject->type.ad.advertisingPrice = getAdvertisingPrice();
				printf("The advertising price has been changed successfully");
				break;
			case ePrintAd:
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

void printAd(const PageObject* pPageObject)
{
	printf("The name of the advertiser of this ad is: %s \n", pPageObject->type.ad.advertiserName);
	printf("The advertising price for this ad is: %d \n", pPageObject->type.ad.advertisingPrice);
	printPageObject(pPageObject);
}

void limitedPrintAd(const PageObject* pPageObject)
{
	printf("Ad named: %s, its advertiser is: %s\n", pPageObject->title, pPageObject->type.ad.advertiserName);
}

void freeAd(PageObject* pPageObject)
{
	freePageObject(pPageObject);
	free(pPageObject->type.ad.advertiserName);
}