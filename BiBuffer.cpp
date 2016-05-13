#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BiBuffer.h"
using namespace std;
Bibuffer::Bibuffer(const char*p)
{
    start_p = 0;
    forward_p = BIBUFFER_SIZE-1;
    memset(BUFFER, 0, BIBUFFER_SIZE*sizeof(token));
    BUFFER[BIBUFFER_SIZE/2-1].type = EOF;
    BUFFER[BIBUFFER_SIZE-1].type = EOF;
    error.pos = -1; error.type = -1;
    if (p != NULL)
    {
        fp = fopen(p, "r");
    }
    else
    {
        fp = NULL;
        cout << "Warning: file name is not specified" << endl;
    }
}

//Error after reduction: may be in GOTO ... detect error: 11:34pm
token Bibuffer::get_token()
{
    token tmp_c;
    int tmp_p;
    if (BUFFER[forward_p].type==EOF)
    {
        if (forward_p == BIBUFFER_SIZE-1)
        {
            tmp_p = 0;
            while(fscanf(fp, "(%d, %d)\n", &tmp_c.type, &tmp_c.pos) != EOF && tmp_p != BIBUFFER_SIZE/2-1)
            {
                BUFFER[tmp_p++] = tmp_c;
            }
            if (tmp_c.type==EOF)
                BUFFER[tmp_p].type = EOF;
            forward_p = 0;
        }
        else if (forward_p == BIBUFFER_SIZE/2-1)
        {
            tmp_p = BIBUFFER_SIZE/2;
            while (fscanf(fp, "(%d, %d)\n", &tmp_c.type, &tmp_c.pos) != EOF && tmp_p != BIBUFFER_SIZE-1)
            {
                BUFFER[tmp_p++] = tmp_c;
            }
            forward_p ++;
        }
        else
            return error;
    }
    return BUFFER[forward_p++];
}
