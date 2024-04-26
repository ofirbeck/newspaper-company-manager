#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "NewspaperCompany.h"
#include "General.h"


int initCompany(NewspaperCompany* pNewsCompany)
{
	pNewsCompany->name = getStrExactName("Enter company name");
	if (!pNewsCompany->name)
		return 0;
	pNewsCompany->clients = NULL;
	pNewsCompany->unpublishedNewspapers = NULL;
	pNewsCompany->clientsCount = 0;
	pNewsCompany->newspaperCount = 0;
	pNewsCompany->clientsSortType = eNoSortClients;
	return 1;
}


Newspaper* findNewspaperByCode(const NewspaperCompany* pNewsCompany, const char* code)
{
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		if (strcmp(pNewsCompany->unpublishedNewspapers[i]->editionCode, code) == 0)
			return pNewsCompany->unpublishedNewspapers[i];
	}
	return NULL;
}

int findNewspaperIndexByCode(const NewspaperCompany* pNewsCompany, const char* code)
{
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		if (strcmp(pNewsCompany->unpublishedNewspapers[i]->editionCode, code) == 0)
			return i;
	}
	return -1;
}

int checkUniqueCode(const NewspaperCompany* pNewsCompany, const char* code)
{
	if (findNewspaperByCode(pNewsCompany, code) != NULL)
		return 0;
	return 1;
}

int addNewspaperToCompany(NewspaperCompany* pNewsCompany)
{
	Newspaper* pNewspaper = (Newspaper*)calloc(1, sizeof(Newspaper));
	CHECK_RETURN_ZERO(pNewspaper);
	char code[EDITION_CODE_LENGTH + 1];
	while (1)
	{
		getEditionCode(code);
		if (checkUniqueCode(pNewsCompany, code))
			break;
		else
			printf("This code is already in use, please choose an other one\n");
	}
	initNewspaper(pNewspaper, code);
	pNewsCompany->unpublishedNewspapers = (Newspaper**)realloc(pNewsCompany->unpublishedNewspapers, sizeof(Newspaper*) * (pNewsCompany->newspaperCount + 1));
	CHECK_RETURN_ZERO(pNewsCompany->unpublishedNewspapers);
	pNewsCompany->unpublishedNewspapers[pNewsCompany->newspaperCount] = pNewspaper;
	pNewsCompany->newspaperCount++;
	printf("A new newspaper was added successfully, you can choose to edit it now\n");
	return 1;
}

int editNewspaper(NewspaperCompany* pNewsCompany, PageObjectStock* stock)
{
	if (pNewsCompany->newspaperCount == 0)
	{
		printf("There is no newspaper in the system to edit, add one and come back\n");
		return 0;
	}
	limitedPrintCompanyNewspapers(pNewsCompany);
	char code[EDITION_CODE_LENGTH + 1];
	Newspaper* chosenNewspaper;
	while (1)
	{
		printf("Choose the code of the newspaper you wish to edit\n");
		getEditionCode(code);
		chosenNewspaper = findNewspaperByCode(pNewsCompany, code);
		if (chosenNewspaper)
			break;
		printf("Invalid code, please try again\n");
	}
	int choice;
	int stop = 0;
	while (!stop)
	{
		printf("Please choose one of these edit options:\n");
		printf("0 - for adding pages to the newspaper\n");
		printf("1 - for edditing pages in the newspaper\n");
		printf("-1 - to exit\n");
		scanf("%d", &choice);
		if (choice == 0)
		{
			if (!addPages(chosenNewspaper, 0))
				return 0;
		}
		else if (choice == 1)
		{
			if (!editPagesInNewspaper(chosenNewspaper, stock))
				return 0;
		}	
		else if (choice == -1)
			stop = 1;
		else
			printf("Invalid choice\n");
	}
	return 1;
}


int addClientToCompany(NewspaperCompany* pNewsCompany)
{
	pNewsCompany->clients = (Client*)realloc(pNewsCompany->clients, (pNewsCompany->clientsCount + 1) * sizeof(Client));
	CHECK_RETURN_ZERO(pNewsCompany->clients);
	if (!initClient(&pNewsCompany->clients[pNewsCompany->clientsCount]))
	{
		printf("Error in creating a new client");
		return 0;
	}
	pNewsCompany->clientsCount++;
	return 1;
}

int editClients(NewspaperCompany* pNewsCompany)
{
	if (pNewsCompany->clientsCount == 0)
	{
		printf("There are no clients who have a membership yet, add some to the system and come back\n");
		return 0;
	}
	limitedPrintCompanyClients(pNewsCompany);
	int editClientNum = 0;
	while (1)
	{
		printf("Choose which of these clients above you want to edit, enter its chronological number\n");
		scanf("%d", &editClientNum);
		if (editClientNum - 1 >= 0 && editClientNum - 1 < pNewsCompany->clientsCount)
			break;
	}
	editClient(&pNewsCompany->clients[editClientNum - 1]);
	return 1;
}

int deleteNewspaperByCode(NewspaperCompany* pNewsCompany, const char* code)
{
	int indexToDelete = findNewspaperIndexByCode(pNewsCompany, code);
	if (indexToDelete == -1)
		return 0;
	Newspaper* temp = pNewsCompany->unpublishedNewspapers[indexToDelete];
	pNewsCompany->unpublishedNewspapers[indexToDelete] = pNewsCompany->unpublishedNewspapers[pNewsCompany->newspaperCount - 1];
	pNewsCompany->unpublishedNewspapers[pNewsCompany->newspaperCount - 1] = temp;
	freeNewspaper(pNewsCompany->unpublishedNewspapers[pNewsCompany->newspaperCount - 1]);
	free(pNewsCompany->unpublishedNewspapers[pNewsCompany->newspaperCount - 1]);
	pNewsCompany->newspaperCount--;
	return 1;
}


int publishNewspaper(NewspaperCompany* pNewsCompany)
{
	if (pNewsCompany->newspaperCount == 0)
	{
		printf("There are no newspapers to publish in the system\n");
		printf("Please add one and come back\n");
		return 0;
	}
	limitedPrintCompanyNewspapers(pNewsCompany);
	char code[EDITION_CODE_LENGTH + 1];
	Newspaper* chosenNewspaper;
	while (1)
	{
		printf("Please enter a code of the newspaper you want to publish\n");
		printf("Please note that you wouldn't be able to edit it anymore\n");
		getEditionCode(code);
		chosenNewspaper = findNewspaperByCode(pNewsCompany, code);
		if (chosenNewspaper)
			break;
	}
	char* fileName = (char*)calloc(MAX_STR_LEN, sizeof(char));
	CHECK_RETURN_ZERO(fileName);
	strcat(fileName, code);
	strcat(fileName, END_OF_PUBLISHED_NEWSPAPER_NAME);
	FILE* fp = fopen(fileName, "w+");
	CHECK_RETURN_ZERO(fp);
	if (fprintf(fp, "This newspaper is brought to you by: %s\n", pNewsCompany->name) < 0)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (!writeNewspaperToTextFile(chosenNewspaper, fp, 1))
	{
		CLOSE_RETURN_ZERO(fp);
	}
	fclose(fp);
	printf("The newspaper has been published! A published version of it has been saved\n");
	printf("You can view it in the file: %s\n", fileName);
	int incomeOrOutcomeFromPaper = getNewspaperIncomeOrOutcome(chosenNewspaper);
	if(incomeOrOutcomeFromPaper <= 0)
		printf("This newspaper has costed the company %d%c to publish\n", -1 * incomeOrOutcomeFromPaper, CURRENCY);
	else
		printf("This newspaper has earned the company %d%c from publishing it\n", incomeOrOutcomeFromPaper, CURRENCY);

	free(fileName);
	return deleteNewspaperByCode(pNewsCompany, chosenNewspaper->editionCode);
}



void sortClients(NewspaperCompany* pNewsCompany)
{
	if (pNewsCompany->clientsCount == 0)
	{
		printf("There are no clients to sort, add some to the system and come back\n");
		return;
	}
	printf("Base on what field do you want to sort the clients?\n");
	while (1)
	{
		printf("Enter 1 for name\nEnter 2 for membership experation date\nEnter 3 for address\n");
		scanf("%d", &pNewsCompany->clientsSortType);
		if (pNewsCompany->clientsSortType > 0 && pNewsCompany->clientsSortType < eNofClientsSortTypes)
			break;
	}
	int(*compare)(const void*, const void*) = NULL;
	switch (pNewsCompany->clientsSortType)
	{
	case eSortClientsByName:
		compare = compareClientsByName;
		break;
	case eSortClientsByDate:
		compare = compareClientsByExpDate;
		break;
	case eSortClientsByAddress:
		compare = compareClientsByAddress;
		break;
	}
	qsort(pNewsCompany->clients, pNewsCompany->clientsCount, sizeof(Client), compare);
	printf("Now the clients are sorted like this:\n");
	limitedPrintCompanyClients(pNewsCompany);
}

void findClient(const NewspaperCompany* pNewsCompany)
{
	if (pNewsCompany->clientsSortType == eNoSortClients)
	{
		printf("%s\n", eClientsSortTypeFindMsg[eNoSortClients]);
		return;
	}
	printf("%s\n", eClientsSortTypeFindMsg[pNewsCompany->clientsSortType]);
	Client searchClient;
	if (!initEmptyClient(&searchClient))
	{
		printf("Error finding a client\n");
		return;
	}
	int(*compare)(const void*, const void*) = NULL;
	switch (pNewsCompany->clientsSortType)
	{
	case eSortClientsByName:
		searchClient.name = getPersonName("client");
		compare = compareClientsByName;
		break;
	case eSortClientsByDate:
		getCorrectDate(searchClient.membershipExperationDate);
		compare = compareClientsByExpDate;
		break;
	case eSortClientsByAddress:
		initAddress(&searchClient.address);
		compare = compareClientsByAddress;
		break;
	}
	Client* foundClient = (Client*)bsearch(&searchClient, pNewsCompany->clients, pNewsCompany->clientsCount, sizeof(Client), compare);
	if (!foundClient)
		printf("Client was not found\n");
	else
	{
		printf("Client found, ");
		printClient(foundClient);
	}
	freeClient(&searchClient);
}

int writeCompanyToTextFile(const NewspaperCompany* pNewsCompany, char* fileName)
{
	FILE* fp = fopen(fileName, "w+");
	CHECK_RETURN_ZERO(fp);
	if (fprintf(fp, "%s\n", pNewsCompany->name) < 0)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (fprintf(fp, "%d\n", pNewsCompany->clientsSortType) < 0)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (fprintf(fp, "%d\n", pNewsCompany->clientsCount) < 0)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	for (int i = 0; i < pNewsCompany->clientsCount; i++)
	{
		if (!writeClientToTextFile(&pNewsCompany->clients[i], fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}
	if (fprintf(fp, "%d\n", pNewsCompany->newspaperCount) < 0)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		if (!writeNewspaperToTextFile(pNewsCompany->unpublishedNewspapers[i], fp, 0))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}
	fclose(fp);
	return 1;
}

int writeCompanyToBinaryFile(const NewspaperCompany* pNewsCompany, char* fileName)
{
	FILE* fp = fopen(fileName, "w+b");
	CHECK_RETURN_ZERO(fp);
	int NameLen = (int)(strlen(pNewsCompany->name) + 1);
	if (fwrite(&NameLen, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (fwrite(pNewsCompany->name, sizeof(char), NameLen, fp) != NameLen)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (fwrite(&pNewsCompany->clientsSortType, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (fwrite(&pNewsCompany->clientsCount, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	for (int i = 0; i < pNewsCompany->clientsCount; i++)
	{
		if (!writeClientToBinaryFile(&pNewsCompany->clients[i], fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}
	if (fwrite(&pNewsCompany->newspaperCount, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		if (!writeNewspaperToBinaryFile(pNewsCompany->unpublishedNewspapers[i], fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}
	fclose(fp);
	return 1;
}

int readCompanyFromTextFile(NewspaperCompany* pNewsCompany, char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	CHECK_RETURN_ZERO(fp);
	char tempName[MAX_STR_LEN] = { 0 };
	do {
		fMyGets(fp, tempName, MAX_STR_LEN);
	} while (strlen(tempName) < 2);
	pNewsCompany->name = _strdup(tempName);

	if (fscanf(fp, "%d", &pNewsCompany->clientsSortType) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (fscanf(fp, "%d", &pNewsCompany->clientsCount) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	pNewsCompany->clients = (Client*)malloc(pNewsCompany->clientsCount * sizeof(Client));
	CHECK_CLOSE_RETURN_ZERO(pNewsCompany->clients, fp);
	for (int i = 0; i < pNewsCompany->clientsCount; i++)
	{
		if (!readClientFromTextFile(&pNewsCompany->clients[i], fp))
		{
			free(pNewsCompany->clients);
			CLOSE_RETURN_ZERO(fp);
		}
	}
	if (fscanf(fp, "%d", &pNewsCompany->newspaperCount) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	pNewsCompany->unpublishedNewspapers = (Newspaper**)malloc(pNewsCompany->newspaperCount * sizeof(Newspaper*));
	CHECK_CLOSE_RETURN_ZERO(pNewsCompany->unpublishedNewspapers, fp);
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		pNewsCompany->unpublishedNewspapers[i] = (Newspaper*)calloc(1, sizeof(Newspaper));
		CHECK_CLOSE_RETURN_ZERO(pNewsCompany->unpublishedNewspapers[i], fp);
		if (!readNewspaperFromTextFile(pNewsCompany->unpublishedNewspapers[i], fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}
	fclose(fp);
	return 1;
}


int readCompanyFromBinaryFile(NewspaperCompany* pNewsCompany, char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	CHECK_RETURN_ZERO(fp);
	int nameLen;
	if (fread(&nameLen, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	pNewsCompany->name = (char*)malloc(nameLen * sizeof(char));
	CHECK_CLOSE_RETURN_ZERO(pNewsCompany->name, fp);
	if (fread(pNewsCompany->name, sizeof(char), nameLen, fp) != nameLen)
	{
		free(pNewsCompany->name);
		CLOSE_RETURN_ZERO(fp);
	}
	if (fread(&pNewsCompany->clientsSortType, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	if (fread(&pNewsCompany->clientsCount, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	pNewsCompany->clients = (Client*)malloc(pNewsCompany->clientsCount * sizeof(Client));
	CHECK_CLOSE_RETURN_ZERO(pNewsCompany->clients, fp);
	for (int i = 0; i < pNewsCompany->clientsCount; i++)
	{
		if (!readClientFromBinaryFile(&pNewsCompany->clients[i], fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}
	
	if (fread(&pNewsCompany->newspaperCount, sizeof(int), 1, fp) != 1)
	{
		CLOSE_RETURN_ZERO(fp);
	}
	pNewsCompany->unpublishedNewspapers = (Newspaper**)malloc(pNewsCompany->newspaperCount * sizeof(Newspaper*));
	CHECK_CLOSE_RETURN_ZERO(pNewsCompany->unpublishedNewspapers, fp);
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		pNewsCompany->unpublishedNewspapers[i] = (Newspaper*)calloc(1, sizeof(Newspaper));
		CHECK_CLOSE_RETURN_ZERO(pNewsCompany->unpublishedNewspapers[i], fp);
		if (!readNewspaperFromBinaryFile(pNewsCompany->unpublishedNewspapers[i], fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}
	fclose(fp);
	return 1;
}


void limitedPrintCompanyNewspapers(const NewspaperCompany* pNewsCompany)
{
	printf("There are %d unpublished newspapers in the system:\n\n", pNewsCompany->newspaperCount);
	int incomeOrOutcomeFromPaper;
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		incomeOrOutcomeFromPaper = (NEWSPAPER_PRINTING_OUTCOME * pNewsCompany->clientsCount) + getNewspaperIncomeOrOutcome(pNewsCompany->unpublishedNewspapers[i]);
		if(incomeOrOutcomeFromPaper <= 0)
			printf("This newspaper will cost the company: %d%c when it will be published\n", -1 * incomeOrOutcomeFromPaper, CURRENCY);
		else
			printf("This newspaper will earn the company: %d%c when it will be published\n", incomeOrOutcomeFromPaper, CURRENCY);
		limitedPrintNewspaper(pNewsCompany->unpublishedNewspapers[i]);
		printf("\n");
	}
}

void limitedPrintCompanyClients(const NewspaperCompany* pNewsCompany)
{
	printf("There are %d clients in the system:\n", pNewsCompany->clientsCount);
	for (int i = 0; i < pNewsCompany->clientsCount; i++)
	{
		printf("%d.", i+1);
		printClient(&pNewsCompany->clients[i]);
	}
}

void printCompanyReport(const NewspaperCompany* pNewsCompany)
{
	printf("------------------------------------------------\n");
	printf("Here is a report of the entire %s company:\n", pNewsCompany->name);
	limitedPrintCompanyNewspapers(pNewsCompany);
	printf("------------------------------------------------\n");
	printf("The monthly fee for a membership is: %d%c\n", MONTHLY_PRICE_FOR_CLIENT, CURRENCY);
	printf("Monthly income from all these clients is: %d%c\n", MONTHLY_PRICE_FOR_CLIENT * pNewsCompany->clientsCount, CURRENCY);
	limitedPrintCompanyClients(pNewsCompany);
	printf("------------------------------------------------\n");
}

void freeCompany(NewspaperCompany* pNewsCompany)
{
	free(pNewsCompany->name);
	generalArrayFunction(pNewsCompany->clients, pNewsCompany->clientsCount, sizeof(Client), freeClientV);
	free(pNewsCompany->clients);
	for (int i = 0; i < pNewsCompany->newspaperCount; i++)
	{
		freeNewspaper(pNewsCompany->unpublishedNewspapers[i]);
		free(pNewsCompany->unpublishedNewspapers[i]);
	}
	free(pNewsCompany->unpublishedNewspapers);
}
