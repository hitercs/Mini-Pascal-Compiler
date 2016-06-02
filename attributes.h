#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H
#include "DataStructure.h"
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
    Attributes* attr_ptr;
    GraAttrNode()
    {
        gra_code = 0;
        attr_ptr = NULL;
    }
}GraAttrNode;
#endif // ATTRIBUTES_H
