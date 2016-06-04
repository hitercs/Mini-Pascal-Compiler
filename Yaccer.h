#ifndef YACCER_H
#define YACCER_H
#include <Stack>
#include "DataStructure.h"
#include "mystack.h"
#include "attributes.h"
#include <fstream>
#include "Lexer.h"
#include "string"
#include <vector>
#include <algorithm>
#define STATUS_NUM      500
#define TERMINAL_NUM    100
#define VAR_NUM         50
#define ERROR           0
#define ACC             -100000
using namespace std;
class Yaccer{
public:
    Yaccer(Lexer& fromlex, const char* file_str=NULL, const char* prod_file=NULL, const char* ter_file=NULL, const char* pro_str_file=NULL);
    void import_production(const char* pro_str); // Read production lines
    void install_table(const char* file_str);    // install goto table and action table
    void LR_analysis(const char* token_file);    // LR analysis
private:
    char iToA[20];
	string intToString(int i)
	{
		sprintf(iToA, "%d", i);
		return string(iToA);
	}
	void write_back_file();
    vector<int> merge_list(vector<int> a, vector<int> b);
    void backpatch(vector<int> list, int quad);
    vector<int> makelist(int quad);
    int nextquad;
    symbol* lookup(char key_[]);
    void newtmp(char ret[]);
    void readProstr(const char* file_str);
    void readTerstr(const char* file_str);
    HierachSymbols* mktable(HierachSymbols* fa_ptr);
    void enter(HierachSymbols* ptr, char str[], int t, int off);
    void addwidth(HierachSymbols* ptr, int w);
    void enterproc(HierachSymbols* ptr, char name[], HierachSymbols* child);
    Lexer lex;
    HierachSymbols* SymbolTableRoot;
    int GOTO_TABLE[STATUS_NUM][VAR_NUM];
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
    int current_level;
    MyStack<int> StatusStack;
    //MyStack<int> GrammarStack;
    MyStack<int> offset;
    MyStack<HierachSymbols*> tblptr;
    MyStack<GraAttrNode> GraAttrStack;
    int curTmp;
    FILE* out;
};
#endif // YACCER_H
