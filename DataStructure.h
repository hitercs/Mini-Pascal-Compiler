#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
// Max length for the string
#define MAX_LEN     512
#define VAR_LEN     100
#define INT_SIZE    200
#define FLOAT_SIZE  200
#define STRING_SIZE 200
#define MAX_CHILD_N 20 // 最大嵌套层数
#include <string.h>
#include "hash.h"
#include "attributes.h"
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
    int type;              // token type code
    int xpos;              // token position
    int ypos;
}token;
// production[VAR_LEN][0] is body size
// production[VAR_LEN][1] is header variable


typedef struct threeAddrCode{
    int op;
    int arg1_addr;  //操作数在符号表中的位置
    int arg2_addr;
    int result_addr;
}threeAddrCode;
typedef struct SymHeader{
    int level;
    int total_varsize;
    SymHeader()
    {
        level = -1;
        total_varsize = 0;
    }
}SymHeader;
typedef struct HierachSymbols{
    SymHeader header;
    Hash<symbol> body;
    HierachSymbols* father;
    typedef struct SymTailRecord{
        char childname[MAX_LEN];
        HierachSymbols* child;
    }SymTailRecord;
    SymTailRecord childs[MAX_CHILD_N];
    HierachSymbols()
    {
        father = NULL;
        for (int i=0;i<MAX_CHILD_N;i++)
            childs[i].child = NULL;
    }
}HierachSymbols;
#endif // DATASTRUCTURE_H
