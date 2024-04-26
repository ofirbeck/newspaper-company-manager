#pragma once
#include <stdio.h>
#include "Article.h"
#include "Ad.h"

#define MAX_TITLE_LENGTH 50
#define MAX_PAGEOBJECT_CONTENT_LEN 10000
#define START_OF_AD_IN_FILE "#start of ad#\n"
#define START_OF_ARTICLE_IN_FILE "#start of article#\n"

typedef enum
{
	eChangetitle, eChangeContent, eAddLines,
	eNofOptionsEditPageObject
} eEditPageObjectMenuOptions;


static const char* eEditPageObjectMenuOptionsStrings[eNofOptionsEditPageObject] = { "Change title", "Change content(entirely)",
											"Add Lines to the start or the end of the content"};


typedef struct PageObject_ {
	void (*print)(const struct PageObject_*); // Function pointer to print
	void (*limitedPrint)(const struct PageObject_*); // Function pointer to limited print
	int (*writeToTextFile)(const struct PageObject_*, FILE*, int); // Function write to text file
	int (*writeToBinaryFile)(const struct PageObject_*, FILE*); // Function write to binary file
	int (*readFromTextFile)(struct PageObject_*, FILE*); // Function read from text file
	int (*readFromBinaryFile)(struct PageObject_*, FILE*); // Function read from binary file
	void (*copy)(struct PageObject_*, struct PageObject_*); // Function copy object one to another
	int (*getIncomeOrOutcome)(const struct PageObject_*); // Function returns the income(+) or the outcome(-) from the object
	void (*edit)(struct PageObject_*, char** titles, int sizeOfTitles); // Function edit the object
	void (*free)(struct PageObject_*); // Function free the object
	char* title;
	char* content;
	union {
		Article article;
		Ad ad;
	} type;
} PageObject;

// PageObject funcs
char* getTitle();
char* getContent();
void copyTitleAndContent(PageObject* destPageObject, PageObject* sourcePageObject);
int writePageObjectToTextFile(const PageObject* pPageObject, FILE* fp, int toPublish);
int writePageObjectToBinaryFile(const PageObject* pPageObject, FILE* fp);
int readPageObjectFromTextFile(PageObject* pPageObject, FILE* fp);
int readPageObjectFromBinaryFile(PageObject* pPageObject, FILE* fp);
char* addLinesToContent(char* oldContent);
int isTitleInTitleArr(const char** titles, int sizeOfTitles, const char* titleToSearch);
void editPageObject(PageObject* pPageObject, char** titles, int sizeOfTitles, int option);
void limitedPrintPageObjectPtr(void* pPageObjectPtr);
void printPageObject(const PageObject* pPageObject);
void freePageObjectPtr(void* pPageObjectPtr);
void freePageObject(PageObject* pPageObject);


// Article funcs
int getArticleOutcome(const PageObject* pPageObject);
void setArticleFuncs(PageObject* pPageObject);
int initArticle(PageObject* pPageObject, char** titles, int sizeOfTitles);
void copyArticles(PageObject* destPageObject, PageObject* sourcePageObject);
int writeArticleToTextFile(const PageObject* pPageObject, FILE* fp, int toPublish);
int writeArticleToBinaryFile(const PageObject* pPageObject, FILE* fp);
int readArticleFromTextFile(PageObject* pPageObject, FILE* fp);
int readArticleFromBinaryFile(PageObject* pPageObject, FILE* fp);
int editArticleOptionsMenu();
void editArticle(PageObject* pPageObject, char** titles, int sizeOfTitles);
void printArticle(const PageObject* pPageObject);
void limitedPrintArticle(const PageObject* pPageObject);
void freeArticle(PageObject* pPageObject);


// Ad funcs
int getAdIncome(const PageObject* pPageObject);
char* getAdvertiserName();
int getAdvertisingPrice();
void setAdFuncs(PageObject* pPageObject);
int initAd(PageObject* pPageObject, char** titles, int sizeOfTitles);
void copyAds(PageObject* destPageObject, PageObject* sourcePageObject);
int writeAdToTextFile(const PageObject* pPageObject, FILE* fp, int toPublish);
int writeAdToBinaryFile(const PageObject* pPageObject, FILE* fp);
int readAdFromTextFile(PageObject* pPageObject, FILE* fp);
int readAdFromBinaryFile(PageObject* pPageObject, FILE* fp);
int editAdOptionsMenu();
void editAd(PageObject* pPageObject, char** titles, int sizeOfTitles);
void printAd(const PageObject* pPageObject);
void limitedPrintAd(const PageObject* pPageObject);
void freeAd(PageObject* pPageObject);