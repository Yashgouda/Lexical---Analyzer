#include "header.h"

extern int line;

char *keywords[32] = {
    "int","float","char","double","signed","unsigned","long",
    "short","volatile","const","for","while","do","break","goto","continue",
    "if","else","switch","case","default","auto","static","register",
    "extern","struct","union","enum","typedef","void","return","sizeof"
};

/*---------------------------------------------------------
                IDENTIFIER / KEYWORD
----------------------------------------------------------*/
void identifier_keyword(char *str)
{
    if (strlen(str) > 1 && keyword(str))
    {
        printf("%-22s : %s\n", "Keyword", str);
    }
    else
    {
        printf("%-22s : %s\n", "Identifier", str);
    }
}

/*---------------------------------------------------------
                KEYWORD CHECK
----------------------------------------------------------*/
int keyword(const char *str)
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(keywords[i], str) == 0)
            return 1;
    }
    return 0;
}

/*---------------------------------------------------------
                HEXADECIMAL CHECK
----------------------------------------------------------*/
int hexadecimal(const char *str)
{
    for (int i = 2; str[i]; i++)
    {
        char c = str[i];
        if (!((c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') ||
              (c >= 'A' && c <= 'F')))
            return 1;
    }
    return 0;
}

/*---------------------------------------------------------
                BINARY CHECK
----------------------------------------------------------*/
int binary(const char *str)
{
    for (int i = 2; str[i]; i++)
    {
        if (str[i] != '0' && str[i] != '1')
            return 1;
    }
    return 0;
}

/*---------------------------------------------------------
                OCTAL CHECK
----------------------------------------------------------*/
int octal(const char *str)
{
    for (int i = 1; str[i]; i++)
    {
        if (str[i] < '0' || str[i] > '7')
            return 1;
    }
    return 0;
}
