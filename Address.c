#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Address.h"
#include "General.h"


int initAddress(Address* pAddress)
{
	while (1)
	{
		pAddress->city = getStrExactName("Please enter your city name\n");
		if (pAddress->city)
			break;
	}
	while (1)
	{
		pAddress->street = getStrExactName("Please enter your street name\n");
		if (pAddress->street)
			break;
	}
	while (1)
	{
		printf("Please enter your home number");
		scanf("%d", &pAddress->homeNumber);
		if (pAddress->homeNumber >= 0)
			break;
	}
	return 1;
}


int writeAddressToTextFile(const Address* pAddress, FILE* fp)
{
	return (fprintf(fp, "%s\n%s\n%d\n", pAddress->city, pAddress->street, pAddress->homeNumber) >= 0);
}

int writeAddressToBinaryFile(const Address* pAddress, FILE* fp)
{
	int cityLen = (int)(strlen(pAddress->city) + 1);
	if (fwrite(&cityLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pAddress->city, sizeof(char), cityLen, fp) != cityLen)
		return 0;
	int streetLen = (int)(strlen(pAddress->street) + 1);
	if (fwrite(&streetLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(pAddress->street, sizeof(char), streetLen, fp) != streetLen)
		return 0;
	if (fwrite(&pAddress->homeNumber, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int readAddressFromTextFile(Address* pAddress, FILE* fp)
{
	char cityNameTemp[MAX_STR_LEN] = { 0 };
	do {
		fMyGets(fp, cityNameTemp, MAX_STR_LEN);
	} while (strlen(cityNameTemp) < 2);

	pAddress->city = _strdup(cityNameTemp);

	if (!pAddress->city)
		return 0;

	char streetNameTemp[MAX_STR_LEN] = { 0 };
	do {
		fMyGets(fp, streetNameTemp, MAX_STR_LEN);
	} while (strlen(streetNameTemp) < 2);

	pAddress->street = _strdup(streetNameTemp);

	if (!pAddress->street)
		return 0;

	if (fscanf(fp, "%d", &pAddress->homeNumber) != 1)
		return 0;
	return 1;
}

int readAddressFromBinaryFile(Address* pAddress, FILE* fp)
{
	int cityLen = 0;
	if (fread(&cityLen, sizeof(int), 1, fp) != 1)
		return 0;

	pAddress->city = (char*)malloc(cityLen * sizeof(char));
	CHECK_RETURN_ZERO(pAddress->city);

	if (fread(pAddress->city, sizeof(char), cityLen, fp) != cityLen)
	{
		free(pAddress->city);
		return 0;
	}
	int streetLen = 0;
	if (fread(&streetLen, sizeof(int), 1, fp) != 1)
		return 0;

	pAddress->street = (char*)malloc(streetLen * sizeof(char));
	CHECK_RETURN_ZERO(pAddress->street);

	if (fread(pAddress->street, sizeof(char), streetLen, fp) != streetLen)
	{
		free(pAddress->street);
		return 0;
	}
	if (fread(&pAddress->homeNumber, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

void printAddress(const Address* pAddress)
{
	printf("The city: %s in the street: %s, house number: %d\n", pAddress->city, pAddress->street, pAddress->homeNumber);
}

void freeAddress(Address* pAddress)
{
	free(pAddress->city);
	free(pAddress->street);
}