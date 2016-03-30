#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
// Max length for the string
#define MAX_LEN     512
#define VAR_LEN     100
#define BODY_LEN    500
#define SHIFT   0
#define REDUCE  1
#define ACC     2
#define ERROR   3
#define EMPTY   4
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

typedef struct Production{
    char Header[VAR_LEN];   // Production left side
    int num;                // number of character of right side
    char Body[BODY_LEN];    // Production right side
    Production()
    {
        memset(Header, 0, VAR_LEN*sizeof(char));
        memset(Body, 0, BODY_LEN*sizeof(char));
        num = 0;
    }
}Production;

typedef struct action_ele{
    int action;
    int pro_num;
    action_ele()
    {
        action = EMPTY; pro_num = 1;
    }
}action_ele;

#endif // DATASTRUCTURE_H
