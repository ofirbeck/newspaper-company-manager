#pragma once
#include "PageObject.h"
#include "listGen.h"

typedef struct PageObjectStock_ {
	LIST pageObjectList;
}PageObjectStock;

int initPageObjectStock(PageObjectStock* stock);
int getStockCount(const PageObjectStock* stock);
char** getTitleArrFromStock(const PageObjectStock* stock);
PageObject* findObjectByTitle(const PageObjectStock* stock, const char* titleToSearch);
int writeStockToTextFile(const PageObjectStock* pStock, const char* fileName);
int writeStockToBinaryFile(const PageObjectStock* pStock, const char* fileName);
int initPageObjectStockFromTextFile(PageObjectStock* stock, const char* fileName);
int initPageObjectStockFromBinaryFile(PageObjectStock* stock, const char* fileName);
int addObjectToStock(PageObjectStock* stock);
int editObjectInStock(PageObjectStock* stock);
int deleteObjectFromStock(PageObjectStock* stock, PageObject* toDelete);
void limitedPrintStock(const PageObjectStock* stock);
void freeObjectStock(PageObjectStock* stock);
