#ifndef BIBUFFER_H
#define BIBUFFER_H
#define BIBUFFER_SIZE   1024
#include <stdio.h>
class Bibuffer{
public:
    Bibuffer(const char*p=NULL);
    int get_char();
    void get_str(int *ret);
private:
    FILE* fp;
    int start_p;
    int forward_p;
    int BUFFER[BIBUFFER_SIZE];
};
#endif // BIBUFFER_H
