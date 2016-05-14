#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H
#include "DataStructure.h"
class Attributes{
public:
    Attributes();
    void set_attr(char* name, int type, void* val);
    attr_node* search_attr(char* name);
    ~Attributes();
private:
    void set_new_attr(char* name, int type, void* val);
    attr_node* header;
};
#endif // ATTRIBUTES_H
