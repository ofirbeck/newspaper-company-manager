#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '#'

void getCorrectDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("Enter Date dd%c%cmm%c%cyyyy  minimum year %d\t",
			SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, MIN_YEAR);
		myGets(date, MAX_STR_LEN);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 12)
		return 0;
	if ((date[2] != SPECIAL_TAV) || (date[3] != SPECIAL_TAV)
		|| (date[6] != SPECIAL_TAV) || (date[7] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%*c%d%*c%*c%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

int writeDateToTextFile(const Date* pDate, FILE* fp)
{
	return (fprintf(fp, "%d\n%d\n%d\n", pDate->day, pDate->month, pDate->year) >= 0);
}

int writeDateToBinaryFile(const Date* pDate, FILE* fp)
{
	if (fwrite(&pDate->day, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&pDate->month, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&pDate->year, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int readDateFromTextFile(Date* pDate, FILE* fp)
{
	if (fscanf(fp, "%d", &pDate->day) != 1)
		return 0;
	if (fscanf(fp, "%d", &pDate->month) != 1)
		return 0;
	if (fscanf(fp, "%d", &pDate->year) != 1)
		return 0;
	return 1;
}

int readDateFromBinaryFile(Date* pDate, FILE* fp)
{
	if (fread(&pDate->day, sizeof(int), 1, fp) != 1)
		return 0;
	if (fread(&pDate->month, sizeof(int), 1, fp) != 1)
		return 0;
	if (fread(&pDate->year, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

void printDate(const Date* pDate)
{
	printf("Date: %d/%d/%d\n", pDate->day, pDate->month, pDate->year);
}
