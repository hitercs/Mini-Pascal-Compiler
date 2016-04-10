#ifndef BIBUFFER_H
#define BIBUFFER_H
#define BIBUFFER_SIZE   1024
#include <stdio.h>
class Bibuffer{
public:
    Bibuffer(const char*p=NULL);
    char get_char();
    void get_str(char *ret);
private:
    FILE* fp;
    int start_p;
    int forward_p;
    char BUFFER[BIBUFFER_SIZE];
};
#endif // BIBUFFER_H
