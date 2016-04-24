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
};
#endif // LEXER_H
