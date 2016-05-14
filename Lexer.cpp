#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Lexer.h"
#include "hash.h"
#include "DataStructure.h"
#include "CodeDef.h"
using namespace std;
FILE* fp = NULL, *out_fp = NULL;
Lexer::Lexer()
{
    line = 0;
    BUFFER[BUFFER_SIZE/2-1] = EOF;
    BUFFER[BUFFER_SIZE-1] = EOF;
    tmp_token[0] = '\0';
    forward_p = BUFFER_SIZE-1;
    start_p = 0;
}
inline int Lexer::is_blank(char c)
{
    if (c == ' '|| c == '\t')
        return 1;
    return 0;
}
inline void Lexer::back_nsteps(int n)
{
    forward_p = (forward_p - n) % BUFFER_SIZE;
}
void Lexer::get_token()
{
    int i;
    for (i = 0; i<=forward_p-start_p-1; i++)
        tmp_token[i] = BUFFER[start_p+i];
    tmp_token[i] = '\0';
}
int Lexer::get_token_type()
{
    key_word* ptr = keywordTable.search_element(tmp_token);
    if (ptr != NULL)
        return ptr->type;
    else
    // Assume all token except keywords are ID
        return ID;
}
void Lexer::id_identify()
{
    int type; // token type
    char tmp_c = BUFFER[forward_p++];
    while (isalnum(tmp_c)||tmp_c=='_')
        tmp_c = BUFFER[forward_p++];
    back_nsteps(1);
    get_token();
    type = get_token_type();
    if (type == ID)
    {
        symbol tmp_s;
        strcpy(tmp_s.name, tmp_token);
        tmp_s.type = type;
        // insert to the symbol
        symbolTable.insert_element(tmp_s);
        fprintf(out_fp, "(%d, %d, %d)\n", type, symbolTable.xpos, symbolTable.ypos);
    }
    else
    {
        fprintf(out_fp, "(%d, %d, %d)\n", type, -1, -1);
    }
    // output
    printf("(%d, %s)\n", type, tmp_token);
    //fprintf(out_fp, "(%d, %d, %d)\n", type, symbolTable.xpos, symbolTable.ypos);
    start_p = forward_p;
}
void Lexer::set_keyword()
{
    FILE* fp = fopen("keyword.txt", "r");
    if (fp == NULL)
    {
        printf("Can't open keyword.txt\n");
        return;
    }
    key_word keys;
    while (fscanf(fp ,"%s %d", keys.name, &keys.type)!=EOF)
    {
        //printf("keys.name = %s, keys.type = %d\n", keys.name, keys.type);
        keywordTable.insert_element(keys);
    }
}
void Lexer::operator_identify()
{
    char tmp_c = BUFFER[forward_p-1];
    switch (tmp_c)
    {
    case '(':
        printf("(%d, %c)\n", LR_BRAC, '(');
        fprintf(out_fp, "(%d, %d, %d)\n", LR_BRAC, -1, -1);
        start_p = forward_p;
        break;
    case ')':
        printf("(%d, %c)\n", RR_BRAC, ')');
        fprintf(out_fp, "(%d, %d, %d)\n", RR_BRAC, -1, -1);
        start_p = forward_p;
        break;
    case '*':
        tmp_c = BUFFER[forward_p++];
        if (tmp_c == '*')
        {
            printf("(%d, %s)\n", EXP, "**");
            fprintf(out_fp, "(%d, %d, %d)\n", EXP, -1, -1);
        }
        else
        {
            printf("(%d, %c)\n", MULTI, '*');
            back_nsteps(1);
            fprintf(out_fp, "(%d, %d, %d)\n", MULTI, -1, -1);
        }
        start_p = forward_p;
        break;
    case ':':
        tmp_c = BUFFER[forward_p++];
        if (tmp_c == '=')
        {
            printf("(%d, %s)\n", ASSIGN, ":=");
            fprintf(out_fp, "(%d, %d, %d)\n", ASSIGN, -1, -1);
        }
        else
        {
            printf("(%d, %c)\n", COLON, ':');
            fprintf(out_fp, "(%d, %d, %d)\n", COLON, -1, -1);
            back_nsteps(1);
        }
        start_p = forward_p;
        break;
    case '<':
        tmp_c = BUFFER[forward_p++];
        if (tmp_c == '=')
        {
            printf("(%d, %s)\n", LE, "<=");
            fprintf(out_fp, "(%d, %d, %d)\n", LE, -1, -1);
        }
        else if (tmp_c == '>')
        {
            printf("(%d, %s)\n", NE, "<>");
            fprintf(out_fp, "(%d, %d, %d)\n", NE, -1, -1);
        }
        else
        {
            printf("(%d, %c)\n", LT, '<');
            fprintf(out_fp, "(%d, %d, %d)\n", LT, -1, -1);
            back_nsteps(1);
        }
        start_p = forward_p;
        break;
    case '=':
        printf("(%d, %c)\n", EQ, '=');
        fprintf(out_fp, "(%d, %d, %d)\n", EQ, -1, -1);
        start_p = forward_p;
        break;
    case '>':
        tmp_c = BUFFER[forward_p++];
        if (tmp_c == '=')
        {
            printf("(%d, %s)", GE, ">=s");
            fprintf(out_fp, "(%d, %d, %d)\n", GE, -1, -1);
        }
        else
        {
            printf("(%d, %c)", GT, '>');
            back_nsteps(1);
            fprintf(out_fp, "(%d, %d, %d)\n", GT, -1, -1);
        }
        start_p = forward_p;
        break;
    case '+':
        printf("(%d, %c)", PLUS, '+');
        fprintf(out_fp, "(%d, %d, %d)\n", PLUS, -1, -1);
        start_p = forward_p;
        break;
    case '-':
        printf("(%d, %c)\n", MINUS, '-');
        fprintf(out_fp, "(%d, %d, %d)\n", MINUS, -1, -1);
        start_p = forward_p;
        break;
    case ',':
        printf("(%d, %c)\n", COMMA, ',');
        fprintf(out_fp, "(%d, %d, %d)\n", COMMA, -1, -1);
        start_p = forward_p;
        break;
    case ';':
        printf("(%d, %c)\n", SEMIC, ';');
        fprintf(out_fp, "(%d, %d, %d)\n", SEMIC, -1, -1);
        start_p = forward_p;
        break;
    case '.':
        printf("(%d, %c)\n", F_STOP, '.');
        fprintf(out_fp, "(%d, %d, %d)\n", F_STOP, -1, -1);
        start_p = forward_p;
        break;
    default:
        start_p++;
        break;
    }
}
void Lexer::comment_identify()
{
    char tmp_c = BUFFER[forward_p-1];
    if (tmp_c == '/')
    {
        tmp_c = BUFFER[forward_p++];
        // Single line comment
        if (tmp_c == '/')
        {
            while(tmp_c != '\n' && tmp_c != EOF)
            {
                tmp_c = BUFFER[forward_p++];
            }
            get_token();
            printf("Comment: %s", tmp_token);
        }
    }
    else
    {
        tmp_c = BUFFER[forward_p++];
        while (tmp_c != '}' && tmp_c != EOF)
        {
            tmp_c = BUFFER[forward_p++];
        }
        if (tmp_c != '}')
            printf("Syntax error: Line %d, Comment } not found", line);
        else
        {
            get_token();
            printf("Comment: %s\n", tmp_token);
        }
    }
    start_p = forward_p;

}
void Lexer::real_identify()
{
    int type = REAL;
    char tmp_c = BUFFER[forward_p-1];
    if (tmp_c == '.')
    {
        tmp_c = BUFFER[forward_p++];
        while(isdigit(tmp_c))
            tmp_c = BUFFER[forward_p++];
    }
    else if (tmp_c == 'E')
    {
        tmp_c = BUFFER[forward_p++];
        if (tmp_c == '+' || tmp_c == '-')
        {
            tmp_c = BUFFER[forward_p++];
            while(isdigit(tmp_c))
                tmp_c = BUFFER[forward_p++];
        }
    }
    back_nsteps(1);
    get_token();
    printf("(%d, %s)\n", type, tmp_token);
    // digits are not saved in the table
    fprintf(out_fp, "(%d, %d, %d)\n", type, -1, -1);
    start_p = forward_p;
}
void Lexer::decimal_identify()
{
    int type = INT;
    char tmp_c = BUFFER[forward_p++];
    while (isdigit(tmp_c))
        tmp_c = BUFFER[forward_p++];
    back_nsteps(1);
    get_token();
    printf("(%d, %s)\n", type, tmp_token);
    // digits are not saved in the table
    fprintf(out_fp, "(%d, %d, %d)\n", type, -1, -1);
    start_p = forward_p;
}
void Lexer::oct_identify()
{
    int type = OCT;
    char tmp_c = BUFFER[forward_p++];
    while (isdigit(tmp_c) && tmp_c>='0' && tmp_c<='7')
        tmp_c = BUFFER[forward_p++];
    back_nsteps(1);
    get_token();
    printf("(%d, %s)\n", type, tmp_token);
    // digits are not saved in the table
    fprintf(out_fp, "(%d, %d, %d)\n", type, -1, -1);
    start_p = forward_p;
}
void Lexer::hex_identify()
{
    int type = HEX;
    char tmp_c = BUFFER[forward_p++];
    while (isdigit(tmp_c) || (isalpha(tmp_c)&&tmp_c>='A'&&tmp_c<='F'))
        tmp_c = BUFFER[forward_p++];
    back_nsteps(1);
    get_token();
    printf("(%d, %s)\n", type, tmp_token);
    // digits are not saved in the table
    fprintf(out_fp, "(%d, %d, %d)\n", type, -1, -1);
    start_p = forward_p;
}
void Lexer::digits_identify()
{
    char tmp_c = BUFFER[forward_p-1];
    if (tmp_c != '0')                     //decimal integer or real number s
    {
        tmp_c = BUFFER[forward_p++];
        while (isdigit(tmp_c))
            tmp_c = BUFFER[forward_p++];
        if (tmp_c == '.' || tmp_c == 'E') //real
        {
            real_identify();
            return;
        }
        else                             // decimal
        {
            decimal_identify();
            return;
        }
    }
    else                                // OCT || HEX || 0 || REAL
    {
        tmp_c = BUFFER[forward_p++];
        if (tmp_c =='X' || tmp_c == 'x')
            hex_identify();
        else if (isdigit(tmp_c) && tmp_c>='0' && tmp_c<='7')
            oct_identify();
        else if (tmp_c == '.')
            real_identify();
        else                    //decimal integer or real
        {
            tmp_c = BUFFER[forward_p++];
            while (isdigit(tmp_c))
                tmp_c = BUFFER[forward_p++];
            if (tmp_c == '.' || tmp_c == 'E')// real
            {
                real_identify();
            }
            else                            //  decimal integer
            {
                decimal_identify();
            }
        }
    }
}
void Lexer::string_identify()
{
    char tmp_c = BUFFER[forward_p];
    while (tmp_c != '\'' && tmp_c!=EOF && tmp_c!='\n')
    {
        tmp_c = BUFFER[forward_p++];
    }
    if (tmp_c == '\'')
    {
        get_token();
        printf("(%d, %s)\n", STRING, tmp_token);
        // not save string to the table
        fprintf(out_fp, "(%d, %d, %d)\n", STRING, -1, -1);
    }
    else
    {
        printf("Syntax error:at %d line, String not closed.\n", line);
    }
    start_p = forward_p;
}
void Lexer::token_analysis(const char* name)
{
    printf("***************************token analysis begin****************************\n");
    int tmp_p = 0;
    char tmp_c;
    if (name == NULL)
    {
        cout << "file name is empty" << endl;
        return;
    }
    fp = fopen(name, "r");
    out_fp = fopen("token.txt", "w");
    if (fp == NULL)
    {
        printf("Can't open sample.c\n");
        exit(-1);
    }
    if (out_fp == NULL)
    {
        printf("Can't open token.txt\n");
    }
    // import keywords to the txt file
    set_keyword();
    while(1)
    {
        if (BUFFER[forward_p] == EOF)
        {
            // 3 cases
            if (forward_p == BUFFER_SIZE/2-1)
            {
                // printf("\n=2=\n");
                // import elements from file to second half of BUFFER
                tmp_p = BUFFER_SIZE/2;
                // Not file end and not buffer end
                while ((tmp_c = fgetc(fp)) != EOF && tmp_p != BUFFER_SIZE-1)
                {
                    printf("%c\t", tmp_c);
                    //printf("%d\t", tmp_p);
                    // pre-processing
                    // 13 ASCii for space
                    if (is_blank(tmp_c))
                    {
                        BUFFER[tmp_p++] = tmp_c;
                        tmp_c = fgetc(fp);
                        while (is_blank(tmp_c))
                        {
                            tmp_c = fgetc(fp);
                            printf("%c\t", tmp_c);
                        }
                        if ((tmp_p < BUFFER_SIZE-1) && tmp_c != EOF)
                            BUFFER[tmp_p++] = tmp_c;
                    }
                    else
                        BUFFER[tmp_p++] = tmp_c;
                }
                if (tmp_c==EOF)
                {
                    BUFFER[tmp_p] = EOF;
                }

                // MOVE start_p
                forward_p ++;
            }
            else if (forward_p == BUFFER_SIZE - 1)
            {
                // import elements from file to first half of BUFFER
                // printf("\n=1=\n");
                tmp_p = 0;
                // Not file end and not buffer end
                while ((tmp_c = fgetc(fp)) != EOF && tmp_p != BUFFER_SIZE/2-1)
                {
                    //printf("%d\t", tmp_p);
                    if (is_blank(tmp_c))
                    {
                        BUFFER[tmp_p++] = tmp_c;
                        tmp_c = fgetc(fp);
                        while (is_blank(tmp_c))
                        {
                            tmp_c = fgetc(fp);
                        }

                        if ((tmp_p < BUFFER_SIZE/2 -1) && tmp_c != EOF)
                        {
                            BUFFER[tmp_p++] = tmp_c;
                        }
                    }
                    else
                        BUFFER[tmp_p++] = tmp_c;
                }
                if (tmp_c==EOF)
                    BUFFER[tmp_p] = EOF;
                // MOVE start_p
                forward_p = 0;
            }
            else
            {
                // FILE end, input over!
                printf("\nFile end!\n");
                break;
            }
        }
        else
        {
            // Read char and processing
            tmp_c = BUFFER[forward_p++];
            //putchar(tmp_c);
            //letter[letter+digit+'_']*
            if (tmp_c == '\n')
                line++;
            if (isalpha(tmp_c))
            {
                id_identify();
            }

            //
            else if (isdigit(tmp_c))
            {
                digits_identify();
            }
            else if (tmp_c == '\'')
            {
                string_identify();
            }
            else if (tmp_c == '/' || tmp_c == '{')
            {
                comment_identify();
            }
            else
            {
                operator_identify();
            }
        }
    }
    printf("Total line is %d\n", line);
    printf("***************************token analysis end*****************************\n");
    fclose(fp);
    fclose(out_fp);
}


