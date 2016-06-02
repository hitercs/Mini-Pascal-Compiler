#include "attributes.h"
#include "CodeDef.h"
#include "stdlib.h"
#include "string.h"
#include "DataStructure.h"
#include <iostream>
using namespace std;

Attributes::Attributes()
{
    header = NULL;
}
void Attributes::set_attr(char* name, int type, void* val)
{
    attr_node* re = search_attr(name);
    char * ptr = NULL;
    if (re==NULL)
        set_new_attr(name, type, val);
    else
    {
        //update value
        switch (type)
        {
        case INT:
            memcpy(re->var_p, val, sizeof(int));
            break;
        case REAL:
            memcpy(re->var_p, val, sizeof(float));
            break;
        case STRING:
            ptr = (char*)val;
            memcpy(re->var_p, val, strlen(ptr)+1);
            break;
        }
    }
}
void Attributes::set_new_attr(char* name, int type, void* val)
{
    char* ptr = NULL;
    attr_node* newNodeP = new attr_node;
    newNodeP->name = name;
    if (header == NULL)
    {
        header = newNodeP;
    }
    else
    {
        attr_node* tmp =header;
        while(tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = newNodeP;
    }
    // allocate memory at run time
    switch (type)
    {
    case INT:
        newNodeP->var_p = malloc(sizeof(int));
        memcpy(newNodeP->var_p, (int*)val, sizeof(int));
        break;
    case REAL:
        newNodeP->var_p = malloc(sizeof(float));
        memcpy(newNodeP->var_p, (float*)val, sizeof(float));
        break;
    case STRING:
        //deep copy
        ptr = (char*)val;
        newNodeP->var_p = malloc(MAX_LEN);
        memcpy(newNodeP->var_p, (char*)val, strlen(ptr)+1);
        break;
    }
}

attr_node* Attributes::search_attr(char* name)
{
    attr_node* tmp = header;
    while(tmp)
    {
        if (!strcmp(tmp->name, name))
        {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}
Attributes::~Attributes()
{
    attr_node* tmp = header;
    attr_node* next= tmp;
    while(next)
    {
        tmp = next;
        next = next->next;
        delete tmp;
    }
}















