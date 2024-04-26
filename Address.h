#pragma once

typedef struct
{
	char* city;
	char* street;
	int	homeNumber;
}Address;

int initAddress(Address* pAddress);
int writeAddressToTextFile(const Address* pAddress, FILE* fp);
int writeAddressToBinaryFile(const Address* pAddress, FILE* fp);
int readAddressFromTextFile(Address* pAddress, FILE* fp);
int readAddressFromBinaryFile(Address* pAddress, FILE* fp);
void printAddress(const Address* pAddress);
void freeAddress(Address* pAddress);