#pragma once
#include <stdio.h>
#include "Date.h"
#include "Address.h"


typedef enum
{
	eChangeClientName, eChangeExpDate, eChangeClientAddress,
	eNofOptionsEditClient
} eEditClientMenuOptions;


static const char* eEditClientMenuOptionsStrings[eNofOptionsEditClient] = { "Change client's name", "Change client's membership experation date",
											"Change client's address" };

typedef struct {
	char* name;
	Date* membershipExperationDate;
	Address address;
}Client;

int initEmptyClient(Client* pClient);
int initClient(Client* pClient);
int writeClientToTextFile(const Client* pClient, FILE* fp);
int writeClientToBinaryFile(const Client* pClient, FILE* fp);
int readClientFromTextFile(Client* pClient, FILE* fp);
int readClientFromBinaryFile(Client* pClient, FILE* fp);
void editClient(Client* pClient);
int	compareClientsByName(const void* client1, const void* client2);
int	compareClientsByExpDate(const void* client1, const void* client2);
int	compareClientsByAddress(const void* client1, const void* client2);
void printClient(const Client* pClient);
void freeClient(Client* pClient);
void freeClientV(void* val);