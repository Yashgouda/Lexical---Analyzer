#include "header.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int flag = 0;
int real_flag = 0, real_flag1 = 0, pre_flag = 0;
static int curve_flag = 0, square_flag = 0, flower_flag = 0;
int line = 1;
int ch; /* use int to hold EOF properly */

int read_fun(const char *code)
{
    FILE *fptr = fopen(code, "r");
    if (fptr == NULL)
    {
        printf("Open failed!\n");
        return 1;
    }

    int in_string = 0;
    int in_char_lit = 0;
    int in_single_comment = 0;
    int in_multi_comment = 0;

    while ((ch = fgetc(fptr)) != EOF)
    {
        char str[256] = {0};

        /* PREPROCESSOR */
        if (ch == '#' && !in_string && !in_char_lit && !in_single_comment && !in_multi_comment)
        {
            /* skip till end of line */
            while ((ch = fgetc(fptr)) != EOF && ch != '\n');
            if (ch == '\n') 
                line++;
            continue;
        }

        /* SINGLE-LINE COMMENT START */
        if (ch == '/' && !in_string && !in_char_lit && !in_multi_comment)
        {
            int nxt = fgetc(fptr);
            if (nxt == '/')
            {
                in_single_comment = 1;
                /* skip the rest of the line (we have entered single-line comment) */
                while ((ch = fgetc(fptr)) != EOF && ch != '\n');
                if (ch == '\n') 
                    line++;
                in_single_comment = 0;
                continue;
            }
            else
            {
                ungetc(nxt, fptr);
            }
        }

        /* MULTI-LINE COMMENT START */
        if (ch == '/' && !in_string && !in_char_lit && !in_single_comment)
        {
            int nxt = fgetc(fptr);
            if (nxt == '*')
            {
                in_multi_comment = 1;
                /* consume until closing */
                int prev = 0;
                while ((ch = fgetc(fptr)) != EOF)
                {
                    if (ch == '\n') 
                        line++;
                    if (prev == '*' && ch == '/')
                    {
                        in_multi_comment = 0;
                        break;
                    }
                    prev = ch;
                }
                continue;
            }
            else
            {
                ungetc(nxt, fptr);
            }
        }

       

        /* STRING LITERAL */
        if (ch == '"' && !in_char_lit && !in_single_comment && !in_multi_comment)
        {
            int i = 0;
            int closed = 0;
            in_string = 1;
            str[i++] = '"';

            while ((ch = fgetc(fptr)) != EOF && i < (int)sizeof(str) - 1)
            {
                str[i++] = (char)ch;

                if (ch == '\\') /* escaped char, include next char as-is */
                {
                    int esc = fgetc(fptr);
                    if (esc == EOF) 
                        break;
                    if (i < (int)sizeof(str) - 1) 
                        str[i++] = (char)esc;
                    if (esc == '\n') 
                        line++;
                    continue;
                }

                if (ch == '"')
                {
                    closed = 1;
                    in_string = 0;
                    break;
                }

                if (ch == '\n')
                {
                    /* newline inside string -> error */
                    break;
                }
            }

            str[i] = '\0';
            if (closed)
            {
                printf("%-22s : %s\n", "String Literal", str);
            }
            else
            {
                printf("%-22s : %s\n", "String Literal", str);
                printf("ERROR : Missing closing double quote (line %d)\n", line);
                /* if we exited due to newline, increment line count already handled below */
            }

            /* if last read char was newline, account for it */
            if (ch == '\n') 
                line++;
            continue;
        }

        /* CHARACTER LITERAL */
        if (ch == '\'' && !in_string && !in_single_comment && !in_multi_comment)
        {
            int i = 0;
            int closed = 0;
            in_char_lit = 1;
            str[i++] = '\'';

            while ((ch = fgetc(fptr)) != EOF && i < (int)sizeof(str) - 1)
            {
                str[i++] = (char)ch;

                if (ch == '\\') /* escaped char inside char literal */
                {
                    int esc = fgetc(fptr);
                    if (esc == EOF) 
                        break;
                    if (i < (int)sizeof(str) - 1) 
                        str[i++] = (char)esc;
                    continue;
                }

                if (ch == '\'')
                {
                    closed = 1;
                    in_char_lit = 0;
                    break;
                }

                if (ch == '\n')
                {
                    /* newline inside char literal -> error */
                    break;
                }
            }

            str[i] = '\0';

            if (!closed)
            {
                printf("ERROR : Missing closing single quote (line %d)\n", line);
            }
            else if (strlen(str) <= 2)
            {
                printf("%-22s : %s\n", "Character Constant", str);
                printf("ERROR : Empty character literal (line %d)\n", line);
            }
            else
            {
                printf("%-22s : %s\n", "Character Constant", str);
            }

            /* if last read char was newline, account for it */
            if (ch == '\n') line++;
            continue;
        }
    
        

        /* IDENTIFIER OR KEYWORD */
        if (isalpha(ch))
        {
            int i = 0;
            str[i++] = (char)ch;

            while ((ch = fgetc(fptr)) != EOF && isalnum(ch))
            {
                if (i < (int)sizeof(str) - 1)
                    str[i++] = (char)ch;
            }

            str[i] = '\0';
            identifier_keyword(str);
            if (ch != EOF) 
                ungetc(ch, fptr);
            continue;
        }

        
        /*  NUMBER TOKEN HANDLING */
        if (isdigit(ch))
        {
            int i = 0;
            flag = 0;
            real_flag1 = 0;

            str[i++] = (char)ch;   // first digit

            while ((ch = fgetc(fptr)) != EOF)
            {
                if (i >= (int)sizeof(str) - 1)
                    break;

                /* HANDLE REAL NUMBER (DOT) */
                if (ch == '.')
                {
                    str[i++] = (char)ch;

                    while ((ch = fgetc(fptr)) != EOF)
                    {
                        if (i >= (int)sizeof(str) - 1)
                            break;

                        if (isdigit(ch))
                        {
                            str[i++] = (char)ch;
                        }

                        /* float suffix (12.3f) */
                        else if (ch == 'f' || ch == 'F')
                        {
                            str[i++] = (char)ch;
                            break;
                        }

                        /* stop at delimiters */
                        else if (strchr(" ;,\n(){}[]", ch))
                        {  
                            break;
                        }

                        /* invalid character inside 12.3g */
                        else
                        {
                            str[i++] = (char)ch;
                            real_flag1 = 1;  // real error
                        }
                    }

                    str[i] = '\0';
                    break;
                }

                /*  DIGIT / HEX / BIN ALLOW */

                /* allow hex digits (0–9, a–f, A–F) */
                if (isxdigit(ch))
                {
                    str[i++] = (char)ch;
                }

                /* allow x X b B only at position 1: 0x12AF / 0b1010 */
                else if (i == 1 && (ch=='x'||ch=='X'||ch=='b'||ch=='B'))
                {
                    str[i++] = (char)ch;
                }

                /* FLOAT SUFFIX only allowed if dot exists */
                else if ((ch=='f'||ch=='F') && strchr(str,'.'))
                {
                    str[i++] = (char)ch;
                    break;
                }

                /* delimiter: end number */
                else if (strchr(" ;,\n(){}[]", ch))
                {
                    break;
                }

                /* invalid number content */
                else
                {
                    str[i++] = (char)ch;
                    flag = 1;   // invalid number
                }
            }

            str[i] = '\0';
            if (ch != EOF)
                ungetc(ch, fptr);

            /* INVALID IDENTIFIER LIKE 1num */
            if (strlen(str) > 1 && isalpha((unsigned char)str[1]) &&
                isdigit((unsigned char)str[0]) && !(str[0]=='0' && (str[1]=='x' || str[1] =='X' || str[1]=='b' || str[1]=='B')))
            {
                printf("ERROR : Identifier cannot start with a digit (line %d)\n", line);
                continue;
            }

            /* INVALID real number (12.3g) */
            if (real_flag1 == 1)
            {
                printf("ERROR! : Invalid Number '%s' (line %d)\n", str, line);
                continue;
            }

            /* GENERAL invalid number (12Z9, 0x12GG) */
            if (flag == 1)
            {
                printf("ERROR : Invalid Number %s (line %d)\n", str, line);
                continue;
            }

            /* ===== VALID NUMBER CLASSIFICATION ===== */

            /* Float / Real */
            if (strchr(str,'.'))
            {
                printf("%-22s : %s\n", "Float constant", str);
                continue;
            }

            /* Hexadecimal */
            if (str[0]=='0' && (str[1]=='x'||str[1]=='X'))
            {
                if (hexadecimal(str))
                {
                    printf("ERROR : ( %s ) Hexadecimal Value Error (line %d)\n", str, line);
                    continue;
                }
                printf("%-22s : %s\n", "Hexadecimal Number", str);
                continue;
            }

            /* Binary */
            if (str[0]=='0' && (str[1]=='b'||str[1]=='B'))
            {
                if (binary(str))
                {
                    printf("ERROR : ( %s ) Binary Value Error! (line %d)\n", str, line);
                    continue;
                }
                printf("%-22s : %s\n", "Binary Number", str);
                continue;
            }

            /* Octal */
            if (str[0]=='0' && strlen(str)>1)
            {
                if (octal(str))
                {
                    printf("ERROR : ( %s ) Octal Value Error (line %d)\n", str, line);
                    continue;
                }
                printf("%-22s : %s\n", "Octal Number", str);
                continue;
            }

            /* Decimal Integer */
            printf("%-22s : %s\n", "Integer Constant", str);
            continue;
        }


        /* BRACKET CHECKING (only when not inside literal/comment) */
        if (!in_string && !in_char_lit && !in_single_comment && !in_multi_comment)
        {
            if (ch == '(') 
                curve_flag++;
            else if (ch == ')') 
                curve_flag--;

            if (ch == '{') 
                flower_flag++;
            else if (ch == '}') 
                flower_flag--;

            if (ch == '[') 
                square_flag++;
            else if (ch == ']') 
                square_flag--;
        }

        /* SPECIAL CHAR */
        if (strchr("(){};[],", ch))
            printf("%-22s : %c\n", "Special Character", ch);

        /* OPERATOR */
        if (strchr("+-*/=<>&|", ch))
            printf("%-22s : %c\n", "Operator", ch);

        /* LINE COUNT */
        if (ch == '\n')
            line++;
    }

    /* FINAL BRACKET ERRORS */
    if (curve_flag != 0)
        printf("ERROR: Unmatched '(' \n");
    if (square_flag != 0)
        printf("ERROR: Unmatched '['\n");
    if (flower_flag != 0)
        printf("ERROR: Unmatched '{'\n");

    fclose(fptr);
    return 0;
}
