#pragma once
#include "Newspaper.h"
#include "Client.h"

#define CURRENCY '$'
#define MONTHLY_PRICE_FOR_CLIENT 50
#define END_OF_PUBLISHED_NEWSPAPER_NAME "publishedNewspaper.txt"


typedef enum {
	eNoSortClients, eSortClientsByName, eSortClientsByDate, eSortClientsByAddress, eNofClientsSortTypes
} eClientsSortType;

static const char* eClientsSortTypeFindMsg[eNofClientsSortTypes]
= { "The search cannot be performed, array not sorted", "The array is sorted by the clients name, pick one to find: ", "The array is sorted by the clients membership eperation date, pick one to find: ", "The array is sorted by the clients address, pick one to find: "};

typedef struct {
	char* name;
	Newspaper** unpublishedNewspapers;
	int newspaperCount;
	Client* clients;
	int clientsCount;
	eClientsSortType clientsSortType;
}NewspaperCompany;

int initCompany(NewspaperCompany* pNewsCompany);
Newspaper* findNewspaperByCode(const NewspaperCompany* pNewsCompany, const char* code);
int findNewspaperIndexByCode(const NewspaperCompany* pNewsCompany, const char* code);
int checkUniqueCode(const NewspaperCompany* pNewsCompany, const char* code);
int addNewspaperToCompany(NewspaperCompany* pNewsCompany);
int editClients(NewspaperCompany* pNewsCompany);
int editNewspaper(NewspaperCompany* pNewsCompany, PageObjectStock* stock);
int addClientToCompany(NewspaperCompany* pNewsCompany);
int deleteNewspaperByCode(NewspaperCompany* pNewsCompany, const char* code);
int publishNewspaper(NewspaperCompany* pNewsCompany);
void sortClients(NewspaperCompany* pNewsCompany);
void findClient(const NewspaperCompany* pNewsCompany);
int writeCompanyToTextFile(const NewspaperCompany* pNewsCompany, char* fileName);
int writeCompanyToBinaryFile(const NewspaperCompany* pNewsCompany, char* fileName);
int readCompanyFromTextFile(NewspaperCompany* pNewsCompany, char* fileName);
int readCompanyFromBinaryFile(NewspaperCompany* pNewsCompany, char* fileName);
void limitedPrintCompanyNewspapers(const NewspaperCompany* pNewsCompany);
void limitedPrintCompanyClients(const NewspaperCompany* pNewsCompany);
void printCompanyReport(const NewspaperCompany* pNewsCompany);
void freeCompany(NewspaperCompany* pNewsCompany);
