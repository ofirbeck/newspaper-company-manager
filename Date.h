#pragma once

#define MIN_YEAR 2024

typedef struct
{
	int	day;
	int	month;
	int	year;
}Date;

void getCorrectDate(Date* pDate);
int	checkDate(char* date, Date* pDate);
int writeDateToTextFile(const Date* pDate, FILE* fp);
int writeDateToBinaryFile(const Date* pDate, FILE* fp);
int readDateFromTextFile(Date* pDate, FILE* fp);
int readDateFromBinaryFile(Date* pDate, FILE* fp);
void printDate(const Date* pDate);
