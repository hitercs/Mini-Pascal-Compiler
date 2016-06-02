#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "BiBuffer.h"
#include "Yaccer.h"
#include "mystack.h"
#include "constStrings.h"
#define ENP 0
#define START_S 0
using namespace std;
extern char pros_str[200][100];
extern char terminals_str[200][50];
Yaccer::Yaccer(const char* file_str, const char* prod_file)
{
    memset(GOTO, 0, STATUS_NUM*VAR_NUM*sizeof(int));
    memset(ACTION, 0, STATUS_NUM*TERMINAL_NUM*sizeof(int));
    if (!(file_str==NULL))
    {
        install_table(file_str);
    }
    if (!(prod_file==NULL))
    {
        import_production(prod_file);
    }
    else
        cout << "Warning: token file name is not specified" << endl;
}
void Yaccer::install_table(const char* file_str)
{
    // install action_table and goto table
    FILE* fp = fopen(file_str, "r");
    int status_n, terminal_n, var_n;
    if (fp==NULL)
    {
        cout << "File: " << file_str << " can't be opened" << endl;
        return;
    }
    // read table size
    fscanf(fp, "%d%d%d", &status_n, &terminal_n, &var_n);
    int i, j;
    // read terminals sign
    for (i=0;i<terminal_n;i++)
        fscanf(fp, "%d", &Terminals[i]);
    // read variable sign
    for (i=0;i<var_n;i++)
        fscanf(fp, "%d", &Vars[i]);
    for (i=0; i<status_n;i++)
    {
        for(j=0;j<terminal_n;j++)
            fscanf(fp, "%d", &ACTION[i][Terminals[j]]);
    }
    //printf("GOTO Table\n");
    for (i=0;i<status_n;i++)
    {
        for(j=0;j<var_n;j++)
        {
            fscanf(fp, "%d", &GOTO[i][-Vars[j]]);
        }
    }
    //printf("goto[4][2] is %d\n", GOTO[4][2]);
}
void Yaccer::import_production(const char* pro_str)
{
    // line start 1
    int line=1;
    int num=0;
    int i=0;
    FILE* fp = fopen(pro_str, "r");
    while(fscanf(fp, "%d", &num)!=EOF)
    {
        production[line][0] = num;
        //fscanf(fp, "%d", &production[line][0]);// read production size
        fscanf(fp, "%d", &production[line][1]);// read production header
        for(i=0;i<production[line][0];i++)
        {
            fscanf(fp, "%d", &production[line][i+2]);
        }
        //line++;
        line++;
    }
}
void Yaccer::LR_analysis(const char* token_file)
{
    printf("*************************LR(1) analysis begin****************************\n");
    StatusStack.push(START_S);
    GrammarStack.push(ENP);
    int top_status = START_S;
    Bibuffer Words(token_file);
    token current_word = Words.get_token();        //warning: char to int Problems char !!
    int ac = ACTION[top_status][current_word.type];
    int reduce_num;
    while (ac != ACC)
    {
        if (ac > 0) // indicate shift
        {
            top_status = ac;
            StatusStack.push(top_status);
            GrammarStack.push(current_word.type);
            cout << "shift: " << terminals_str[current_word.type] << endl;
            current_word = Words.get_token();
        }
        else if (ac==ERROR)
        {
            cout << "error when parsing " << terminals_str[current_word.type] << endl;
            break;
        }
        else
        {
            // reduce with the -ac th production
            // with error
            reduce_num = production[-ac][0];
            StatusStack.npop(reduce_num);
            GrammarStack.npop(reduce_num);
            GrammarStack.push(production[-ac][1]);
            //printf("grammar stack top is %d\n", -production[-ac][1]);
            //printf("status stack top is %d\n",StatusStack.top_ele());
            //printf("x = %d, y = %d, GOTO[X][Y] = %d\n", StatusStack.top_ele(),-production[-ac][1],GOTO[StatusStack.top_ele()][-production[-ac][1]]);
            top_status = GOTO[StatusStack.top_ele()][-production[-ac][1]];
            StatusStack.push(top_status);
            cout << "Reduce with production: " << pros_str[-ac] << endl;
        }
        top_status = StatusStack.top_ele();
        ac = ACTION[top_status][current_word.type];
    }
    if (ac==ACC)
        cout << "parsing succeeded" << endl;
    printf("*************************LR(1) analysis end****************************\n");
    return;
}

















