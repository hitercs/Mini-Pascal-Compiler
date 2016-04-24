#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
// Max length for the string
#define MAX_LEN     512
#define VAR_LEN     100
#include <string.h>
// Key word type def
typedef struct key_word{
    char name[MAX_LEN];
    int type;
    key_word* next;
    key_word()
    {
        name[0] = '\0'; type = -1; next = NULL;
    };
}key_word;
// Symbol type def
typedef struct symbol{
    char name[MAX_LEN];
    unsigned long addr;
    int type;
    void* extend;
    symbol* next;
    symbol()
    {
        name[0] = '\0'; extend = NULL; next = NULL;
        addr = -1; type = -1;
    };
}symbol;

typedef struct token{
    int type;             // token type code
    int pos;              // token position
}token;
// production[VAR_LEN][0] is body size
// production[VAR_LEN][1] is header variable
#endif // DATASTRUCTURE_H
