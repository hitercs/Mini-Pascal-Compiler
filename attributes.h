#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H
#include "DataStructure.h"
using namespace std;
typedef struct attr_node{
    attr_node* next;
    void* var_p;
    char* name;
    attr_node()
    {
        next = NULL; var_p = NULL; name = NULL;
    }
}attr_node;
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
typedef struct GraAttrNode{
    int gra_code;
    int quad;
    int offset;
    Attributes* attr_ptr;
    vector < int > truelist;
    vector < int > falselist;
    vector < int > nextlist;
    string relop_name;
    GraAttrNode()
    {
        gra_code = 0;
        attr_ptr = NULL;
        quad = -1;
    }
}GraAttrNode;
#endif // ATTRIBUTES_H
