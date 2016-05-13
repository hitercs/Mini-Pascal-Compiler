#include <iostream>
#include "Lexer.h"
#include "Yaccer.h"
using namespace std;

int main(int argc, char* agrv[])
{
    Lexer lex;
    lex.token_analysis("quicksort.pas");
    //Yaccer yacc("tables.txt", "productions.txt");
    Yaccer yacc("minipas_table.txt", "pro_code.txt");
    yacc.LR_analysis("token.txt");
    return 0;
}
