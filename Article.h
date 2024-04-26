#pragma once
#include <stdio.h>
#include "Writer.h"

typedef enum
{
	eChangeArticleWriter , ePrintArticle, eNofOptionsEditArticle
} eUniqueEditArticleMenuOptions;


static const char* eUniqueEditArticleMenuOptionsStrings[eNofOptionsEditArticle] = {"Change writer", "Print the article"};

typedef struct {
	Writer artWriter;
}Article;


