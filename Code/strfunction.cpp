#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include "strfunction.h"
using namespace std;
void str2upper(char str[])
{
    int i = 0;
    while (str[i]!='\0')
    {
        str[i] = toupper(str[i]);
        i++;
    }
}
