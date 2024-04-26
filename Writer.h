#pragma once
#define MIN_PAYMENT 0
#define MAX_PAYMENT 511
#define CHECK_VALID_PAYMENT(payment)	(((payment) > (MIN_PAYMENT)) && ((payment) <= (MAX_PAYMENT)))


typedef enum {
	eSport , eEconomy , ePolitics, eNofWritingFields
} eWritingField;

static const char* WriterTypeStr[eNofWritingFields] = { "sports", "economy", "politics" };


typedef struct {
	char* name;
	int paymentForArticle;
	eWritingField	writingField;
}Writer;

int getWriterPaymentForArticle();
eWritingField getWritingField();
int initWriter(Writer* pWriter);
int writeWriterToTextFile(const Writer* pWriter, FILE* fp, int toPublish);
int writeWriterToBinaryFile(const Writer* pWriter, FILE* fp);

int readWriterFromTextFile(Writer* pWriter, FILE* fp);
int readWriterFromBinaryFile(Writer* pWriter, FILE* fp);

void copyWriters(Writer* destWriter, Writer* sourceWriter);
void printWriter(const Writer* pWriter);
void freeWriter(Writer* pWriter);