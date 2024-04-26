#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Client.h"
#include "General.h"


int initEmptyClient(Client* pClient)
{
	pClient->name = NULL;
	pClient->membershipExperationDate = (Date*)malloc(sizeof(Date));
	CHECK_RETURN_ZERO(pClient->membershipExperationDate);
	pClient->address.city = NULL;
	pClient->address.street = NULL;
	pClient->address.homeNumber = 0;
	return 1;
}

int initClient(Client* pClient)
{
	pClient->name = getPersonName("client");
	pClient->membershipExperationDate = (Date*)malloc(sizeof(Date));
	CHECK_RETURN_ZERO(pClient->membershipExperationDate);
	getCorrectDate(pClient->membershipExperationDate);
	
	initAddress(&pClient->address);
	return 1;
}

int writeClientToTextFile(const Client* pClient, FILE* fp)
{
	if (fprintf(fp, "%s\n", pClient->name) < 0)
		return 0;
	if (!writeDateToTextFile(pClient->membershipExperationDate, fp))
		return 0;
	return writeAddressToTextFile(&pClient->address, fp);
}

int writeClientToBinaryFile(const Client* pClient, FILE* fp)
{
	int nameLen = (int)(strlen(pClient->name) + 1);
	if (fwrite(&nameLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pClient->name, sizeof(char), nameLen, fp) != nameLen)
		return 0;
	if (!writeDateToBinaryFile(pClient->membershipExperationDate, fp))
		return 0;
	return writeAddressToBinaryFile(&pClient->address, fp);
}

int readClientFromTextFile(Client* pClient, FILE* fp)
{
	char nameTemp[MAX_STR_LEN] = { 0 };
	do {
		fMyGets(fp, nameTemp, MAX_STR_LEN);
	} while (strlen(nameTemp) < 2);

	pClient->name = _strdup(nameTemp);

	if (!pClient->name)
		return 0;
	pClient->membershipExperationDate = (Date*)malloc(sizeof(Date));
	CHECK_RETURN_ZERO(pClient->membershipExperationDate);
	if (!readDateFromTextFile(pClient->membershipExperationDate, fp))
		return 0;
	return readAddressFromTextFile(&pClient->address, fp);
}

int readClientFromBinaryFile(Client* pClient, FILE* fp)
{
	int nameLen = 0;
	if (fread(&nameLen, sizeof(int), 1, fp) != 1)
		return 0;

	pClient->name = (char*)malloc(nameLen * sizeof(char));
	CHECK_RETURN_ZERO(pClient->name);

	if (fread(pClient->name, sizeof(char), nameLen, fp) != nameLen)
	{
		free(pClient->name);
		return 0;
	}
	pClient->membershipExperationDate = (Date*)malloc(sizeof(Date));
	CHECK_RETURN_ZERO(pClient->membershipExperationDate);
	if (!readDateFromBinaryFile(pClient->membershipExperationDate, fp))
		return 0;
	return readAddressFromBinaryFile(&pClient->address, fp);
}

void editClient(Client* pClient)
{
	int stop = 0;
	int option;
	while (!stop)
	{
		option = menu(eEditClientMenuOptionsStrings, (int)eNofOptionsEditClient);
		switch (option)
		{
		case eChangeClientName:
			free(pClient->name);
			pClient->name = getPersonName("client");
			printf("The client's name changed successfully\n");
			break;
		case eChangeExpDate:
			getCorrectDate(pClient->membershipExperationDate);
			break;
		case eChangeClientAddress:
			freeAddress(&pClient->address);
			initAddress(&pClient->address);
			break;
		case EXIT:
			stop = 1;
			break;
		default:
			printf("Wrong option\n");
			break;
		}
	}
}

int	compareClientsByName(const void* client1, const void* client2)
{
	const Client c1 = *(const Client*)client1;
	const Client c2 = *(const Client*)client2;
	return strcmp(c1.name, c2.name);
}

int	compareClientsByExpDate(const void* client1, const void* client2)
{
	const Client c1 = *(const Client*)client1;
	const Client c2 = *(const Client*)client2;
	int yearDef = c1.membershipExperationDate->year - c2.membershipExperationDate->year;
	int monthDef = c1.membershipExperationDate->month - c2.membershipExperationDate->month;
	int dayDef = c1.membershipExperationDate->day - c2.membershipExperationDate->day;
	if (yearDef > 0)
		return 1;
	else if (yearDef < 0)
		return -1;
	if (monthDef > 0)
		return 1;
	else if (monthDef < 0)
		return -1;
	if (dayDef > 0)
		return 1;
	else if (dayDef < 0)
		return -1;
	return 0;
}

int	compareClientsByAddress(const void* client1, const void* client2)
{
	const Client c1 = *(const Client*)client1;
	const Client c2 = *(const Client*)client2;
	int cityDef = strcmp(c1.address.city, c2.address.city);
	int streetDef = strcmp(c1.address.street, c2.address.street);
	int homenumberDef = c1.address.homeNumber - c2.address.homeNumber;
	if (cityDef > 0)
		return 1;
	else if (cityDef < 0)
		return -1;
	if (streetDef > 0)
		return 1;
	else if (streetDef < 0)
		return -1;
	if (homenumberDef > 0)
		return 1;
	else if (homenumberDef < 0)
		return -1;
	return 0;
}

void printClient(const Client* pClient)
{
	printf("The client is named: %s, and the experation date for their membership is:\n", pClient->name);
	printDate(pClient->membershipExperationDate);
	printf("The client's address is:\n");
	printAddress(&pClient->address);
}

void freeClient(Client* pClient)
{
	free(pClient->name);
	free(pClient->membershipExperationDate);
	freeAddress(&pClient->address);
}


void freeClientV(void* val)
{
	freeClient((Client*)val);
}