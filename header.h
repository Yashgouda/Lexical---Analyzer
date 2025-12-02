#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
extern int line;
// Function declarations
int read_fun(const char *filename);

void identifier_keyword(char *str);
void literal(char *str, char ch, FILE *fptr);

int keyword(const char *str);

int hexadecimal(const char *str);
int binary(const char *str);
int octal(const char *str);

#endif
