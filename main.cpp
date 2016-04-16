#include <iostream>
#include "Lexer.h"
#include "Yaccer.h"
using namespace std;

int main(int argc, char* agrv[])
{
    //Lexer lex;
    //lex.token_analysis();
    Yaccer yacc("tables.txt", "productions.txt");
    yacc.LR_analysis("test_token.txt");
    return 0;
}
