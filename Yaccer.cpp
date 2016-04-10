#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "BiBuffer.h"
#include "Yaccer.h"
#include "mystack.h"
#define ENP -1
#define START_S 0
using namespace std;
Yaccer::Yaccer(const char* file_str, const char* prod_file)
{
    if (!(file_str==NULL))
    {
        install_table(file_str);
    }
    if (!(prod_file==NULL))
    {
        import_production(file_str);
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
    for (i=0; i<status_n;i++)
    {
        for(j=0;j<terminal_n;j++)
            fscanf(fp, "%d", &ACTION[i][j]);
    }
    for (i=0;i<status_n;i++)
    {
        for(j=0;j<terminal_n;j++)
            fscanf(fp, "%d", &GOTO[i][j]);
    }
}
void Yaccer::import_production(const char* pro_str)
{
    int line=0;
    int num=0;
    int i=0;
    FILE* fp = fopen(pro_str, "r");
    while(fscanf(fp, "%d", &num)!=EOF)
    {
        if (num>0)
        {
            fscanf(fp, "%d", &production[line][0]);// read production size
            fscanf(fp, "%d", &production[line][1]);// read production header
            for(i=0;i<production[line][0];i++)
            {
                fscanf(fp, "%d", &production[line][i+2]);
            }
            line++;
        }
    }
}
void Yaccer::LR_analysis(const char* token_file)
{
    StatusStack.push(START_S);
    GrammarStack.push(ENP);
    int top_status = START_S;
    Bibuffer Words(token_file);
    int current_word = Words.get_char(); //warning: char to int
    int ac = ACTION[top_status][current_word];
    int reduce_num;
    while (ac != ACC)
    {
        if (ac > 0) // indicate shift
        {
            top_status = ac;
            StatusStack.push(top_status);
            GrammarStack.push(current_word);
            cout << "shift " << current_word << endl;
            current_word = Words.get_char();
        }
        else if (ac < 0) // indicate reduce
        {
            // reduce with the -ac th production
            reduce_num = production[-ac][0];
            StatusStack.npop(reduce_num);
            GrammarStack.npop(reduce_num);
            GrammarStack.push(production[-ac][1]);
            top_status = GOTO[StatusStack.top_ele()][production[-ac][1]];
            StatusStack.push(top_status);
            cout << "Reduce with production: " << -ac << endl;
            current_word = Words.get_char();
        }
        else if (ac==ERROR)
        {
            cout << "error when parsing" << current_word << endl;
        }
        ac = ACTION[top_status][current_word];
    }
    if (ac==ACC)
        cout << "parsing succeeded" << endl;
    return;
}

















