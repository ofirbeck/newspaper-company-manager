#pragma once
#include <stdio.h>
#define CHECK_VALID_PRICE(price)	(price) > (0)


typedef enum
{
	eChangeAdAdvertiserName, eChangeAdAdvertisingPrice, ePrintAd, eNofOptionsEditAd
} eUniqueEditAdMenuOptions;


static const char* eUniqueEditAdMenuOptionsStrings[eNofOptionsEditAd] = { "Change advertiser name", "Change advertising price", "Print the ad"};


typedef struct {
	char* advertiserName;
	int advertisingPrice;
}Ad;

