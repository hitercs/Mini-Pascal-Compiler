#ifndef BIBUFFER_H
#define BIBUFFER_H
#define BIBUFFER_SIZE   1024
#include <stdio.h>
#include "DataStructure.h"
class Bibuffer{
public:
    Bibuffer(const char*p=NULL);
    token get_token();
private:
    token error;
    FILE* fp;
    int start_p;
    int forward_p;
    token BUFFER[BIBUFFER_SIZE];
};
#endif // BIBUFFER_H
