#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Lexer.h"
#include "hash.h"
#include "DataStructure.h"
#include "CodeDef.h"
using namespace std;
FILE* fp, *out_fp;
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
void Lexer::id_identify()
{

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
        keywordTable.insert_element(keys);
    }
}
void Lexer::operator_identify()
{

}
void Lexer::comment_identify()
{

}
void Lexer::digits_identify()
{

}
void Lexer:: string_identify()
{

}
void Lexer::token_analysis()
{
    int tmp_p = 0;
    char tmp_c;
    fp = fopen("sample.pas", "r");
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
    fclose(fp);
    fclose(out_fp);
}


