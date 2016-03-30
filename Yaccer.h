#ifndef YACCER_H
#define YACCER_H
#define STATUS_NUM  100
#define TERMINAL_NUM    200
#define VAR_NUM         200
#include "DataStructure.h"
class Yaccer{
public:
    Yaccer();
    void install_table();
    void LR_anlysis();
private:
    int GOTO[STATUS_NUM][VAR_NUM];
    action_ele ACTION[STATUS_NUM][TERMINAL_NUM];
};
#endif // YACCER_H
