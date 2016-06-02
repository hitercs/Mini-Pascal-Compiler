#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "BiBuffer.h"
#include "Yaccer.h"
#include "mystack.h"
#include "CodeDef.h"
#include "constStrings.h"
#define ENP 0
#define START_S 0
using namespace std;
extern char pros_str[200][100];
extern char terminals_str[200][50];
Yaccer::Yaccer(Lexer& fromlex, const char* file_str, const char* prod_file)
{
    memset(GOTO_TABLE, 0, STATUS_NUM*VAR_NUM*sizeof(int));
    memset(ACTION, 0, STATUS_NUM*TERMINAL_NUM*sizeof(int));
    lex = fromlex;
    current_level = 0;
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
            fscanf(fp, "%d", &(GOTO_TABLE[i][-Vars[j]]));
        }
    }
    //printf("goto[4][2] is %d\n", GOTO[4][2]);
}
void Yaccer::import_production(const char* pro_str)
{
    // line start 1, 下标从1 开始
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
//--------------------------------------------
//符号表处理函数
HierachSymbols* Yaccer::mktable(HierachSymbols* fa_ptr)
{
    HierachSymbols* newTable;
    if (fa_ptr == NULL)
    {
        newTable = new HierachSymbols;
        return newTable;
    }
    newTable = new HierachSymbols;
    newTable->father = fa_ptr;
    // 保存当前嵌套深度到符号表的头部
    newTable->header.level = current_level;
    return newTable;
}
//--------------------------------------------
void Yaccer::LR_analysis(const char* token_file)
{
    printf("*************************LR(1) analysis begin****************************\n");
    GraAttrNode newNode;
    newNode.gra_code = START_S;
    newNode.attr_ptr = new Attributes;
    GraAttrStack.push(newNode);
    StatusStack.push(ENP);
    int tmp_int;
    float tmp_float;
    char tmp_str[MAX_LEN];
    //GrammarStack.push(ENP);
    int top_status = START_S;
    Bibuffer Words(token_file);
    token current_word = Words.get_token();        //warning: char to int Problems char !!
    int ac = ACTION[top_status][current_word.type];
    int reduce_num;
    HierachSymbols* newTable;
    while (ac != ACC)
    {
        if (ac > 0) // indicate shift
        {
            top_status = ac;
            StatusStack.push(top_status);
            newNode.gra_code = current_word.type;
            // 收集词法分析的属性
            newNode.attr_ptr = new Attributes;
            if (current_word.type == DIG)
            {
                tmp_int = lex.int_consts[current_word.xpos];
                newNode.attr_ptr->set_attr("var", INT, &tmp_int);
            }
            else if (current_word.type == REAL)
            {
                tmp_float = lex.float_consts[current_word.xpos];
                newNode.attr_ptr->set_attr("var", REAL, &tmp_float);
            }
            else if (current_word.type == STRING)
            {
                strcpy(tmp_str, lex.string_consts[current_word.xpos]);
                newNode.attr_ptr->set_attr("var", STRING, tmp_str);
            }
            GraAttrStack.push(newNode);
            //GrammarStack.push(current_word.type);
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
            // -ac 是产生式标号（从0 开始计数），按照不同的产生式规约，应用不同的语义动作
            newNode.gra_code = production[-ac][1];
            newNode.attr_ptr = new Attributes;
            switch (-ac)
            {
            case 1:
                // 用产生式 M-> e 来规约时，新建一个嵌套深度为1的符号表
                newTable = mktable(NULL);
                tblptr.push(newTable);
                offset.push(0);
                break;
            case 19:
                // 用产生式 standard_type => integer 来规约时，添加：属性type 和 width
                tmp_int = INT;
                newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                tmp_int = sizeof(int);
                newNode.attr_ptr->set_attr("width", INT, &tmp_int);
                break;
            case 20:
                // 用产生式 standard_type => real 来规约时，添加：属性type 和 width
                tmp_int = REAL;
                newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                tmp_int = sizeof(float);
                newNode.attr_ptr->set_attr("width", INT, &tmp_int);
                break;
            case 51:
                // 用产生式type -> array [ digits .. digits ] of standard_type 来规约时，添加：综合属性 type.type, type.width
                {
                    int left_dig = *((int*)(((GraAttrStack.top_ele_by_off(-5)).attr_ptr)->search_attr("var")->var_p));
                    int right_dig = *((int*)(((GraAttrStack.top_ele_by_off(-3)).attr_ptr)->search_attr("var")->var_p));
                    int width = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("width")->var_p));
                    tmp_int = (right_dig - left_dig)*width;
                    newNode.attr_ptr->set_attr("width", INT, &tmp_int);
                    tmp_int = ARRAY;
                    newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                    break;
                }


            }
            reduce_num = production[-ac][0];
            StatusStack.npop(reduce_num);
            GraAttrStack.npop(reduce_num);
            //GrammarStack.npop(reduce_num);
            GraAttrStack.push(newNode);
            //GrammarStack.push(production[-ac][1]);
            top_status = GOTO_TABLE[StatusStack.top_ele()][-production[-ac][1]];
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

















