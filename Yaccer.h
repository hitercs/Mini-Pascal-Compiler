#ifndef YACCER_H
#define YACCER_H
#include "DataStructure.h"
#include "mystack.h"
#define STATUS_NUM      1000
#define TERMINAL_NUM    200
#define VAR_NUM         200
#define ERROR           0
#define ACC             -100000
class Yaccer{
public:
    Yaccer(const char* file_str=NULL, const char* prod_file=NULL);
    void import_production(const char* pro_str); // Read production lines
    void install_table(const char* file_str);    // install goto table and action table
    void LR_analysis(const char* token_file);    // LR analysis
private:
    int GOTO[STATUS_NUM][VAR_NUM];
    // + indicate shift
    // - indicate reduce
    // 0 is error
    // -100000 is acc
    //terminal code
    int Terminals[TERMINAL_NUM];
    //var code
    int Vars[VAR_LEN];
    int ACTION[STATUS_NUM][TERMINAL_NUM];
    int production[VAR_LEN][MAX_LEN];
    MyStack<int> StatusStack;
    MyStack<int> GrammarStack;
};
#endif // YACCER_H
