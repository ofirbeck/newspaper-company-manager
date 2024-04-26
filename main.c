#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <crtdbg.h>
#include "PageObjectStock.h"
#include "NewspaperCompany.h"
#include "General.h"

#define _CRTDBG_MAP_ALLOC

#define COMPANY_TEXT_FILE_NAME "NewspaperCompanyText.txt"
#define COMPANY_BINARY_FILE_NAME "NewspaperCompanyBin.bin"
#define STOCK_TEXT_FILE_NAME "stockText.txt"
#define STOCK_BINARY_FILE_NAME "stockBinary.bin"

typedef enum
{
	eAddObjectToStock, eEditObjectInStock, ePrintStock, eAddClient, eEditClient, eSortClients, eFindClient, eCreateNewNewspaper, eEditNewspaper, ePublishNewspaper, eCompanyReport,  eNofMainOptions
} eMainMenuOptions;

static const char* mainMenuStrOptions[eNofMainOptions] = { "Add an article or an ad to the stock", "Edit an article or an ad in stock", "Print stock", "Add client", "Edit client", "Sort clients", "Find client", "Create a new newspaper", "Edit a newspaper", "Publish a newspaper", "Get a company's report - monthly income from users, and future costs of newspapers"};


char chooseFileToReadFrom()
{
	char choice;
	while (1)
	{
		printf("Please choose from which file you want to read from?\n");
		printf("write 't' or 'T' for text file\n");
		printf("or write 'b' or 'B' for binary file\n");
		scanf(" %c", &choice);
		choice = tolower(choice);
		if (choice == 't' || choice == 'b')
			return choice;
		printf("Invalid answer, please try again\n");
	}
}


int initCompanyAndStock(NewspaperCompany* pCompany, PageObjectStock* pStock, char readFileChoice)
{
	int resStock = 0;
	int resCompnay = 0;
	if (readFileChoice == 't')
	{
		resCompnay = readCompanyFromTextFile(pCompany, COMPANY_TEXT_FILE_NAME);
		resStock = initPageObjectStockFromTextFile(pStock, STOCK_TEXT_FILE_NAME);
	}
	else if (readFileChoice == 'b')
	{
		resCompnay = readCompanyFromBinaryFile(pCompany, COMPANY_BINARY_FILE_NAME);
		resStock = initPageObjectStockFromBinaryFile(pStock, STOCK_BINARY_FILE_NAME);
	}
	if (!resStock)
	{
		resStock = initPageObjectStock(pStock);
	}
	if (!resCompnay)
	{
		resCompnay = initCompany(pCompany);
	}
	return (resCompnay && resStock);
}

int main()
{
	PageObjectStock stock;
	NewspaperCompany newspaperCompany;
	char readFileChoice = chooseFileToReadFrom();
	if (!initCompanyAndStock(&newspaperCompany, &stock, readFileChoice))
	{
		printf("Error init\n");
		return;
	}
	int option;
	int stop = 0;
	do
	{
		option = menu(mainMenuStrOptions, (int)eNofMainOptions);
		switch (option)
		{
		case eAddObjectToStock:
			if(!addObjectToStock(&stock))
				printf("Error adding object to stock\n");
			break;
		case eEditObjectInStock:
			if(!editObjectInStock(&stock))
				printf("Error editing object in stock\n");
			break;
		case ePrintStock:
			limitedPrintStock(&stock);
			break;
		case eAddClient:
			if(!addClientToCompany(&newspaperCompany))
				printf("Error adding a new client\n");
			break;
		case eEditClient:
			if(!editClients(&newspaperCompany))
				printf("Error editing client\n");
			break;
		case eSortClients:
			sortClients(&newspaperCompany);
			break;
		case eFindClient:
			findClient(&newspaperCompany);
			break;
		case eCreateNewNewspaper:
			if (!addNewspaperToCompany(&newspaperCompany))
				printf("Error adding a new newspaper\n");
			break;
		case eEditNewspaper:
			if (!editNewspaper(&newspaperCompany, &stock))
				printf("Error editing newspaper\n");
			break;
		case ePublishNewspaper:
			if (!publishNewspaper(&newspaperCompany))
				printf("Error publishing a newspaper\n");
			break;
		case eCompanyReport:
			printCompanyReport(&newspaperCompany);
			break;
		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			writeCompanyToTextFile(&newspaperCompany, COMPANY_TEXT_FILE_NAME);
			writeCompanyToBinaryFile(&newspaperCompany, COMPANY_BINARY_FILE_NAME);
			writeStockToTextFile(&stock, STOCK_TEXT_FILE_NAME);
			writeStockToBinaryFile(&stock, STOCK_BINARY_FILE_NAME);
			freeCompany(&newspaperCompany);
			freeObjectStock(&stock);
			break;
		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);
	_CrtDumpMemoryLeaks();
	return 1;
}
