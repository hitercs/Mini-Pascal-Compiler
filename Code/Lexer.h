#ifndef LEXER_H
#define LEXER_H
#define maxlen 512
#define BUFFER_SIZE 1024
#include "DataStructure.h"
#include "hash.h"
// Define LEX
class Lexer
{
public:
    Lexer();
    void token_analysis(const char* name);
    symbol* search_symbols(int xpos, int ypos);
    //consts tables
    int int_consts[INT_SIZE];
    float float_consts[FLOAT_SIZE];
    char string_consts[STRING_SIZE][MAX_LEN];
private:
    Hash<key_word> keywordTable;
    Hash<symbol> symbolTable;
    inline int is_blank(char c);
    inline void back_nsteps(int n);
    int get_token_type();
    void get_token();
    void id_identify();
    void digits_identify();
    void string_identify();
    void comment_identify();
    void operator_identify();
    void real_identify();
    void decimal_identify();
    void oct_identify();
    void hex_identify();
    void set_keyword();
    char BUFFER[BUFFER_SIZE];
    char tmp_token[maxlen];
    int line;
    int start_p, forward_p;
    int float_n; int int_n; int string_n;
};
#endif // LEXER_H
