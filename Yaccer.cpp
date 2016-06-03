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
#include <fstream>
#include <sstream>
#define ENP 0
#define START_S 0
using namespace std;
char pros_str[200][200];
char terminals_str[200][100];
Yaccer::Yaccer(Lexer& fromlex, const char* file_str, const char* prod_file, const char* ter_file, const char* pro_str_file)
{
    memset(GOTO_TABLE, 0, STATUS_NUM*VAR_NUM*sizeof(int));
    memset(ACTION, 0, STATUS_NUM*TERMINAL_NUM*sizeof(int));
    lex = fromlex;
    current_level = 0;
    curTmp = 0;
    SymbolTableRoot = NULL;
    out = fopen("threeAddr.txt", "w");
    if (!(file_str==NULL))
    {
        install_table(file_str);
    }
    if (!(prod_file==NULL))
    {
        import_production(prod_file);
    }
    if (!(ter_file == NULL))
    {
        readTerstr(ter_file);
    }
    if (!(pro_str_file == NULL))
    {
        readProstr(pro_str_file);
    }
    else
        cout << "Warning: token file name is not specified" << endl;
}

void Yaccer::readProstr(const char* file_str)
{
    FILE* fp = fopen(file_str, "r");
    int n=1;
    if (fp==NULL)
    {
        cout << "File: " << file_str << " can't be opened" << endl;
        return;
    }
    //while(fscanf(fp, "%s\n", pros_str[n++])!= EOF);
    //while(fscanf(fp, "%s\n", terminals_str[n++])!= EOF);
    while(fgets(pros_str[n], 200, fp) != NULL)
    {
        n++;
    };
}

void Yaccer::readTerstr(const char* file_str)
{
    FILE* fp = fopen(file_str, "r");
    int n=0; //��0 ��ʼ����
    if (fp==NULL)
    {
        cout << "File: " << file_str << " can't be opened" << endl;
        return;
    }
    //while(fscanf(fp, "%s\n", terminals_str[n++])!= EOF);
    while(fgets(terminals_str[n], 100, fp) != NULL)
    {
        n++;
    };
}
void Yaccer::newtmp(char ret[])
{
    strcpy(ret, "___tmp");
    char tmp[10];
    itoa(curTmp++, tmp, 10);
    strcat(ret, tmp);
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
    // line start 1, �±��1 ��ʼ
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
//���ű�����
HierachSymbols* Yaccer::mktable(HierachSymbols* fa_ptr)
{
    HierachSymbols* newTable;
    newTable = new HierachSymbols;
    newTable->father = fa_ptr;
    // ���浱ǰǶ����ȵ����ű��ͷ��
    newTable->header.level = current_level;
    printf("[new symbol table]: Level: %d\n", newTable->header.level);
    return newTable;
}
void Yaccer::enter(HierachSymbols* ptr, char str[], int t, int off)
{
    if (ptr)
    {
        symbol tmp_s;
        strcpy(tmp_s.name, str);
        tmp_s.type = t;
        tmp_s.addr = off;
        ptr->body.insert_element(tmp_s);
    }
    else
    {
        printf("Error: ptr is null!\n");
    }
}
void Yaccer::addwidth(HierachSymbols* ptr, int w)
{
    if (ptr)
    {
        ptr->header.total_varsize = w;
    }
    else
        printf("Error: ptr is null!\n");
}
symbol* Yaccer::lookup(char key_[])
{
    HierachSymbols* current_tb = tblptr.top_ele();
    if (current_tb != NULL)
    {
        while (current_tb!=NULL && ((current_tb->body.search_element(key_))==NULL))
        {
            current_tb = current_tb->father;
        }
        if (current_tb == NULL)
            return NULL;
        return current_tb->body.search_element(key_);
    }
    return NULL;
}
void Yaccer::enterproc(HierachSymbols* ptr, char name[], HierachSymbols* child)
{
    if (ptr == NULL)
        return;
    int i;
    for (i=0; i<MAX_CHILD_N; i++)
    {
        if (ptr->childs[i].child == NULL)
            break;
    }
    if (ptr->childs[i].child == NULL)
    {
        strcpy(ptr->childs[i].childname, name);
        ptr->childs[i].child = child;
    }
    else
    {
        printf("Symbol table childs full\n");
    }
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
    MyStack<GraAttrNode> ID_STACK;
    while (ac != ACC)
    {
        if (ac > 0) // indicate shift
        {
            top_status = ac;
            StatusStack.push(top_status);
            newNode.gra_code = current_word.type;
            // �ռ��ʷ�����������
            newNode.attr_ptr = new Attributes;
            if (current_word.type == INT)
            {
                tmp_int = lex.int_consts[current_word.xpos];
                newNode.attr_ptr->set_attr("var", INT, &tmp_int);
                tmp_int = INT;
                newNode.attr_ptr->set_attr("type", INT, &tmp_int);
            }
            else if (current_word.type == REAL)
            {
                tmp_float = lex.float_consts[current_word.xpos];
                newNode.attr_ptr->set_attr("var", REAL, &tmp_float);
                tmp_int = REAL;
                newNode.attr_ptr->set_attr("type", INT, &tmp_int);
            }
            else if (current_word.type == STRING)
            {
                strcpy(tmp_str, lex.string_consts[current_word.xpos]);
                newNode.attr_ptr->set_attr("var", STRING, tmp_str);
                tmp_int = STRING;
                newNode.attr_ptr->set_attr("type", INT, &tmp_int);
            }
            else if (current_word.type == ID)
            {
                strcpy(tmp_str, lex.search_symbols(current_word.xpos, current_word.ypos)->name);
                newNode.attr_ptr->set_attr("name", STRING, tmp_str);
            }
            GraAttrStack.push(newNode);
            //GrammarStack.push(current_word.type);
            cout << "shift: " << terminals_str[current_word.type] ;
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
            // -ac �ǲ���ʽ��ţ���1 ��ʼ�����������ղ�ͬ�Ĳ���ʽ��Լ��Ӧ�ò�ͬ�����嶯��
            newNode.gra_code = production[-ac][1];
            newNode.attr_ptr = new Attributes;

            switch (-ac)
            {
            //**********************�������������ķ���********************************
            case 1:
                // �ò���ʽ M-> e ����Լʱ���½�һ��Ƕ�����Ϊ1�ķ��ű�
                current_level ++;
                newTable = mktable(NULL);
                tblptr.push(newTable);
                SymbolTableRoot = newTable;
                offset.push(0);
                break;
            case 19:
                // �ò���ʽ standard_type => integer ����Լʱ����ӣ�����type �� width
                tmp_int = INT;
                newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                tmp_int = sizeof(int);
                newNode.attr_ptr->set_attr("width", INT, &tmp_int);
                break;
            case 20:
                // �ò���ʽ standard_type => real ����Լʱ����ӣ�����type �� width
                tmp_int = REAL;
                newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                tmp_int = sizeof(float);
                newNode.attr_ptr->set_attr("width", INT, &tmp_int);
                break;
            case 66:
                // �ò���ʽtype -> array [ integer .. integer ] of standard_type ����Լʱ����ӣ��ۺ����� type.type, type.width
                {
                    int left_dig = *((int*)(((GraAttrStack.top_ele_by_off(-5)).attr_ptr)->search_attr("var")->var_p));
                    int right_dig = *((int*)(((GraAttrStack.top_ele_by_off(-3)).attr_ptr)->search_attr("var")->var_p));
                    int width = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("width")->var_p));
                    tmp_int = (right_dig - left_dig+1)*width;
                    newNode.attr_ptr->set_attr("width", INT, &tmp_int);
                    tmp_int = ARRAY;
                    newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                    break;
                }
            case 18:
                // �ò���ʽ type -> standard_type ����Լʱ���������Դ���
                {
                    int width = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("width")->var_p));
                    int type = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("type")->var_p));
                    newNode.attr_ptr->set_attr("width", INT, &width);
                    newNode.attr_ptr->set_attr("type", INT, &type);
                    break;
                }
            case 4:
                // �ò���ʽ identifier_list -> id ����Լʱ������id ��ID ջ����
                {
                    ID_STACK.push(GraAttrStack.top_ele());
                    break;
                }
            case 16:
                // �ò���ʽ identifier_list -> identifier_list, id ����Լʱ������id ��ID ջ����
                {
                    ID_STACK.push(GraAttrStack.top_ele());
                    break;
                }
            case 17:
                // �ò���ʽ declaration -> identifier_list : type ����Լʱ������ű�
                {
                    HierachSymbols* current_tb = tblptr.top_ele();
                    int type, width;
                    while(!ID_STACK.is_empty())
                    {
                        type = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("type")->var_p));
                        width = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("width")->var_p));
                        enter(current_tb, (char*)((ID_STACK.top_ele().attr_ptr)->search_attr("name")->var_p), type, offset.top_ele());
                        width += offset.top_ele();
                        offset.npop(1);
                        offset.push(width);
                        ID_STACK.npop(1);
                    }
                    break;
                }
             case 50:
                // �ò���ʽdeclaration -> declaration semi identifier_list : type ����Լʱ������ű�
                {
                    HierachSymbols* current_tb = tblptr.top_ele();
                    int type, width;
                    while(!ID_STACK.is_empty())
                    {
                        type = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("type")->var_p));
                        width = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("width")->var_p));
                        enter(current_tb, (char*)((ID_STACK.top_ele().attr_ptr)->search_attr("name")->var_p), type, offset.top_ele());
                        width += offset.top_ele();
                        offset.npop(1);
                        offset.push(width);
                        ID_STACK.npop(1);
                    }
                    break;
                }
             case 21:
                //�ò���ʽ subprogram_declaration -> subprogram_head declarations compound_statement ����Լ��������ű�ͷ
                {
                    HierachSymbols* current_tb = tblptr.top_ele();
                    addwidth(current_tb, offset.top_ele());
                    tblptr.npop(1);
                    offset.npop(1);
                    strcpy(tmp_str,(char*)((GraAttrStack.top_ele_by_off(-2).attr_ptr)->search_attr("name")->var_p));
                    //xx Ӧ���滻�� subprogram_head.name
                    if (tblptr.is_empty())
                        enterproc(NULL, tmp_str, current_tb);
                    else
                        enterproc(tblptr.top_ele(), tmp_str, current_tb);
                    current_level--;
                    break;
                }
             case 62:
                {
                    //�ò���ʽ subprogram_head -> function id arguments : standard_type semi ����Լʱ���½�һ�����ű�
                    current_level++;
                    newTable = mktable(tblptr.top_ele());
                    tblptr.push(newTable);
                    offset.push(0);
                    //����id ��name ��subprogram_head��
                    strcpy(tmp_str,(char*)((GraAttrStack.top_ele_by_off(-4).attr_ptr)->search_attr("name")->var_p));
                    newNode.attr_ptr->set_attr("name", STRING, tmp_str);
                    break;
                }
             case 49:
                {
                    //�ò���ʽsubprogram_head -> procedure id arguments semi ����Լ�� �½�һ�����ű�
                    current_level++;
                    newTable = mktable(tblptr.top_ele());
                    tblptr.push(newTable);
                    offset.push(0);
                    //����id ��name ��subprogram_head��
                    strcpy(tmp_str,(char*)((GraAttrStack.top_ele_by_off(-2).attr_ptr)->search_attr("name")->var_p));
                    newNode.attr_ptr->set_attr("name", STRING, tmp_str);
                    break;
                }
             case 5:
                {
                    //�ò���ʽprogram -> prog id ( input , output ) semi M declarations subprogram_declarations compound_statement����Լʱ�������ű�ͷ��
                    HierachSymbols* current_tb = tblptr.top_ele();
                    addwidth(current_tb, offset.top_ele());
                    tblptr.npop(1);
                    offset.npop(1);
                    current_level--;
                    break;
                }
              //**********************�������������ķ���********************************
              //**********************�����Ǹ�ֵ���ķ���********************************
             case 26:
                 // �ò���ʽ factor -> id ����Լʱ��ͨ��ID.name ����ű�
                {
                    strcpy(tmp_str,(char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("name")->var_p));
                    symbol* ptr = lookup(tmp_str);
                    if (ptr == NULL)
                    {
                        printf("[Error:] %s is undefined\n", tmp_str);
                        break;
                    }
                    strcpy(tmp_str, ptr->name);
                    // set id.name to factor.addr
                    newNode.attr_ptr->set_attr("addr", STRING, tmp_str);
                    break;
                }
             case 27:
                // �ò���ʽ num -> integer ����Լʱ
                {
                    int type, value;
                    type = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("type")->var_p));
                    value = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("var")->var_p));
                    printf("value is %d\n", value);
                    newNode.attr_ptr->set_attr("type", INT, &type);
                    newNode.attr_ptr->set_attr("val", INT, &value);
                    break;
                }
             case 28:
                 // �ò���ʽ num -> real ����Լʱ
                {
                    int type;
                    type = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("type")->var_p));
                    tmp_float = *((float*)((GraAttrStack.top_ele().attr_ptr)->search_attr("var")->var_p));
                    newNode.attr_ptr->set_attr("type", INT, &type);
                    newNode.attr_ptr->set_attr("val", REAL, &tmp_float);
                    break;
                }
             case 25:
                 // ���ò���ʽ factor -> num ����Լʱ
                {
                    //char newlabel[MAX_LEN];
                    //newtmp(newlabel);
                    char var[20];
                    //printf("new str is %s\n", newlabel);
                    int type;
                    type = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("type")->var_p));
                    if (type == INT)
                    {
                        tmp_int = *((int*)((GraAttrStack.top_ele().attr_ptr)->search_attr("val")->var_p));
                        itoa(tmp_int, var, 10);
                    }
                    else if (type == REAL)
                    {
                        tmp_float = *((float*)((GraAttrStack.top_ele().attr_ptr)->search_attr("val")->var_p));
                        gcvt(tmp_float, 5, var);
                    }
                    else
                        strcpy(var, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("val")->var_p));
                    //fprintf(out, "%s := %s\n", newlabel, var);
                    newNode.attr_ptr->set_attr("addr", STRING, var);
                    break;
                }
             case 24:
                 // �ò���ʽ term -> factor ����Լʱ
                {
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("addr")->var_p));
                    newNode.attr_ptr->set_attr("addr", STRING, tmp_str);
                    break;
                }
             case 55:
                 // �ò���ʽ term -> term mul_div_op factor ����Լʱ, �������ַ��
                {
                    char newlabel[MAX_LEN];
                    newtmp(newlabel);
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("addr")->var_p));
                    char tmp_str2[MAX_LEN];
                    strcpy(tmp_str2, (char*)((GraAttrStack.top_ele_by_off(-2).attr_ptr)->search_attr("addr")->var_p));
                    int type = *((int*)((GraAttrStack.top_ele_by_off(-1).attr_ptr)->search_attr("type")->var_p));
                    if (type == MULTI)
                        fprintf(out, "%s := %s * %s", newlabel, tmp_str, tmp_str2);
                    else if (type == DIV)
                        fprintf(out, "%s := %s / %s", newlabel, tmp_str, tmp_str2);
                    newNode.attr_ptr->set_attr("addr", STRING, newlabel);
                    break;
                }
             case 45:
                // �ò���ʽ mul_div_op -> * ʱ�� ����op-type
                {
                    tmp_int = MULTI;
                    newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                    break;
                }
             case 46:
                // �ò���ʽ mul_div_op -> / ʱ�� ����op-type
                {
                    tmp_int = DIV;
                    newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                    break;
                }
             case 23:
                // �ò���ʽ simple_expression -> term ʱ�� ����op-type
                {
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("addr")->var_p));
                    newNode.attr_ptr->set_attr("addr", STRING, tmp_str);
                    break;
                }
             case 29:
                 //�ò���ʽ sign -> + ʱ, ���� sign ������
                {
                    tmp_int = PLUS;
                    newNode.attr_ptr->set_attr("sign", INT, &tmp_int);
                    break;
                }
             case 30:
                 //�ò���ʽ sign -> - ʱ, ���� sign ������
                {
                    tmp_int = MINUS;
                    newNode.attr_ptr->set_attr("sign", INT, &tmp_int);
                    break;
                }
              case 37:
                // �ò���ʽ plus_min_op -> + ʱ�� ����op-type
                {
                    tmp_int = PLUS;
                    newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                    break;
                }
                case 38:
                // �ò���ʽ plus_min_op -> - ʱ�� ����op-type
                {
                    tmp_int = MINUS;
                    newNode.attr_ptr->set_attr("type", INT, &tmp_int);
                    break;
                }
                case 47:
                //�ò���ʽ��simple_expression -> sign term ����Լʱ
                {
                    char newlabel[MAX_LEN];
                    newtmp(newlabel);
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("addr")->var_p));
                    int sign = *((int*)((GraAttrStack.top_ele_by_off(-1).attr_ptr)->search_attr("sign")->var_p));
                    if (sign == PLUS)
                        fprintf(out, "%s := %s", newlabel, tmp_str);
                    else if (sign == MINUS)
                        fprintf(out, "%s := - %s", newlabel, tmp_str);
                    newNode.attr_ptr->set_attr("addr", STRING, newlabel);
                    break;
                }
                case 54:
                // �ò���ʽ��simple_expression -> simple_expression plus_min_op term ����Լ
                {
                    char newlabel[MAX_LEN];
                    newtmp(newlabel);
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("addr")->var_p));
                    char tmp_str2[MAX_LEN];
                    strcpy(tmp_str2, (char*)((GraAttrStack.top_ele_by_off(-2).attr_ptr)->search_attr("addr")->var_p));
                    int type = *((int*)((GraAttrStack.top_ele_by_off(-1).attr_ptr)->search_attr("type")->var_p));
                    if (type == PLUS)
                        fprintf(out, "%s := %s + %s\n", newlabel, tmp_str, tmp_str2);
                    else if (type == MINUS)
                        fprintf(out, "%s := %s - %s\n", newlabel, tmp_str, tmp_str2);
                    newNode.attr_ptr->set_attr("addr", STRING, newlabel);
                    break;
                }
                case 35:
                //�ò���ʽ��expression -> simple_expression ����Լʱ
                {
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("addr")->var_p));
                    newNode.attr_ptr->set_attr("addr", STRING, tmp_str);
                    break;
                }
                case 14:
                //�ò���ʽ��variable -> id ����Լʱ
                {
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("name")->var_p));
                    newNode.attr_ptr->set_attr("addr", STRING, tmp_str);
                    tmp_int = -1;
                    newNode.attr_ptr->set_attr("offset", INT, &tmp_int);
                    break;
                }
                case 34:
                //�ò���ʽ��statement -> variable assignop expression ����Լʱ
                {
                    strcpy(tmp_str, (char*)((GraAttrStack.top_ele().attr_ptr)->search_attr("addr")->var_p));
                    char tmp_str2[MAX_LEN];
                    strcpy(tmp_str2, (char*)((GraAttrStack.top_ele_by_off(-2).attr_ptr)->search_attr("addr")->var_p));
                    int offset_ = *((int*)((GraAttrStack.top_ele_by_off(-2).attr_ptr)->search_attr("offset")->var_p));
                    if (offset_ == -1)
                        //simple var
                        fprintf(out, "%s := %s \n", tmp_str2, tmp_str);
                    else
                        fprintf(out, "%s[%d] := %s\n", tmp_str2, offset_, tmp_str);
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
            cout << "Reduce with production: " << pros_str[-ac] ;
        }
        top_status = StatusStack.top_ele();
        ac = ACTION[top_status][current_word.type];
    }
    if (ac==ACC)
        cout << "\nparsing succeeded" << endl;
    printf("*************************LR(1) analysis end****************************\n");
    fclose(out);
    return;
}

















