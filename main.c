/*
 * Project : Lexical Analyzer for C Language
 * Author  : Yashavanth G Gouda
 *
 * Description :
 * --------------------------------------------------------------
 * This project implements a Lexical Analyzer (Lexer) for the C
 * programming language. The lexer scans an input C source file
 * character by character and breaks it into a sequence of
 * meaningful tokens. It also performs basic error detection for
 * invalid tokens.
 */
#include "header.h"

int main(int argc, char *argv[])
{
    /* Check if input file is provided */
    if (argc < 2)
    {
        printf("Error: Missing input file.\n");
        printf("Usage: ./lexer <filename>\n");
        return 1;
    }
    /* Call lexical analyzer function */
    if (read_fun(argv[1]) != 0)
    {
        printf("Error: Could not process file '%s'\n", argv[1]);
        return 1;
    }

    return 0;
}
