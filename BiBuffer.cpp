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
    memset(BUFFER, 0, BIBUFFER_SIZE*sizeof(int));
    BUFFER[BIBUFFER_SIZE/2-1] = EOF;
    BUFFER[BIBUFFER_SIZE-1] = EOF;
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

int Bibuffer::get_char()
{
    int tmp_c=EOF;
    int tmp_p;
    if (BUFFER[forward_p]==EOF)
    {
        if (forward_p == BIBUFFER_SIZE-1)
        {
            tmp_p = 0;
            while(fscanf(fp, "%d", &tmp_c) != EOF && tmp_p != BIBUFFER_SIZE/2-1)
            {
                BUFFER[tmp_p++] = tmp_c;
            }
            if (tmp_c==EOF)
                BUFFER[tmp_p] = EOF;
            forward_p = 0;
        }
        else if (forward_p == BIBUFFER_SIZE/2-1)
        {
            tmp_p = BIBUFFER_SIZE/2;
            while (fscanf(fp, "%d", &tmp_c) != EOF && tmp_p != BIBUFFER_SIZE-1)
            {
                BUFFER[tmp_p++] = tmp_c;
            }
            forward_p ++;
        }
        else
            return -1;
    }
    return BUFFER[forward_p++];
}
void Bibuffer::get_str(int* ret)
{
    int i;
    for (i = 0; i<=forward_p-start_p-1; i++)
        ret[i] = BUFFER[start_p+i];
}
