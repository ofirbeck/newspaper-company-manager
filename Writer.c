#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Writer.h"
#include "General.h"


int getWriterPaymentForArticle()
{
	int payment;
	while (1)
	{
		printf("Enter writer's payment for its article\n");
		printf("Please note it should not be under the min payment: %d\n", MIN_PAYMENT);
		printf("and not above the max payment: %d\n", MAX_PAYMENT);
		scanf("%d", &payment);
		if(CHECK_VALID_PAYMENT(payment))
			break;
		else
			printf("Invalid payment, please try again\n");
	}
	return payment;
}



int initWriter(Writer* pWriter)
{
	pWriter->name = getPersonName("writer");
	pWriter->paymentForArticle = getWriterPaymentForArticle();
	pWriter->writingField = getWritingField();
	return 1;
}

eWritingField getWritingField()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofWritingFields; i++)
			printf("%d for %s\n", i, WriterTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofWritingFields);
	getchar();
	return (eWritingField)option;
}

int writeWriterToTextFile(const Writer* pWriter, FILE* fp, int toPublish)
{
	if (toPublish)
		return (fprintf(fp, "Written by: %s Which specialize in: %s\n", pWriter->name, WriterTypeStr[pWriter->writingField]) >= 0);
	else
		return (fprintf(fp, "%s\n%d\n%d\n", pWriter->name, pWriter->paymentForArticle, pWriter->writingField) >= 0);

}

int writeWriterToBinaryFile(const Writer* pWriter, FILE* fp)
{
	BYTE data[2] = { 0 };
	int len = (int)strlen(pWriter->name);
	data[0] = (pWriter->paymentForArticle >> 8) | (pWriter->writingField << 1) | (len << 3);
	data[1] = pWriter->paymentForArticle;
	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;
	if (fwrite(pWriter->name, sizeof(char), len, fp) != len)
		return 0;
	return 1;
}


int readWriterFromTextFile(Writer* pWriter, FILE* fp)
{
	char writerNameTemp[MAX_STR_LEN] = { 0 };
	do {
		fMyGets(fp, writerNameTemp, MAX_STR_LEN);
	} while (strlen(writerNameTemp) < 2);
	if (fscanf(fp, "%d", &pWriter->paymentForArticle) != 1)
		return 0;
	if (fscanf(fp, "%d", &pWriter->writingField) != 1)
		return 0;
	pWriter->name = _strdup(writerNameTemp);
	if (!pWriter->name)
		return 0;
	return 1;

}

int readWriterFromBinaryFile(Writer* pWriter, FILE* fp)
{
	BYTE data[2];
	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;
	int len = (data[0] >> 3) & 0x1F;
	pWriter->writingField = (data[0] >> 1) & 0x3;
	pWriter->paymentForArticle = ((data[0] & 0x1) << 8) | (data[1]);
	pWriter->name = (char*)calloc(len + 1, sizeof(char));
	CHECK_RETURN_ZERO(pWriter->name);

	if (fread(pWriter->name, sizeof(char), len, fp) != len)
	{
		free(pWriter->name);
		return 0;
	}
	return 1;
}


void copyWriters(Writer* destWriter, Writer* sourceWriter)
{
	destWriter->name = (char*)malloc(sizeof(char) * (strlen(sourceWriter->name) + 1));
	if (!destWriter->name)
		return;
	strcpy(destWriter->name, sourceWriter->name);
	destWriter->paymentForArticle = sourceWriter->paymentForArticle;
	destWriter->writingField = sourceWriter->writingField;
}

void printWriter(const Writer* pWriter)
{
	printf("Writer's name: %s, it's payment for its article: %d, it's writing field: %s\n", pWriter->name, pWriter->paymentForArticle, WriterTypeStr[pWriter->writingField]);
}

void freeWriter(Writer* pWriter)
{
	free(pWriter->name);
}