#include <iostream>
#include "Lexer.h"
#include "Yaccer.h"
#include "CodeDef.h"
#include <stdio.h>
using namespace std;

int main(int argc, char* agrv[])
{
    //Lexer lex;
    //lex.token_analysis("sample.pas");
    //lex.token_analysis("quicksort.pas");
    //lex.token_analysis("tmp.pas");
    //lex.token_analysis("quicksort_emample.pas");
    //Yaccer yacc("tables.txt", "productions.txt");
    //Yaccer yacc(lex, "minipas_table.txt", "pro_code.txt");
    //yacc.LR_analysis("token.txt");
    Attributes list_;
    int offset = 10;
    float var  = 1.0;
    char str[] = "hello";
    list_.set_attr("offset", INT, &offset);
    list_.set_attr("var", REAL, &var);
    list_.set_attr("str", STRING, str);
    printf("offset is %d", *((int *)(list_.search_attr("offset")->var_p)));
    printf("var is %d", *((float *)(list_.search_attr("offset")->var_p)));
    printf("str is %s", ((char *)(list_.search_attr("str")->var_p)));
    return 0;
}
