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
    int offset;         // 数组翻译使用
    char addr[MAX_LEN]; //变量的名字
    int type;
    int width;
    float var_float;
    int var_int;
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
