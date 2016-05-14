#include <iostream>
#include <string.h>
#include "hash.h"
#include "DataStructure.h"
#include "strfunction.h"
using namespace std;
//Hash function
unsigned int SDBMHash(const char *str)
{
    unsigned int hash = 0;
    const char* tmp = str;
    while (*tmp)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = *(tmp++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF)%BUCKET_SIZE;
}

//Hash definition
template <class T>
Hash<T>::Hash()
{
    memset(Array, 0, BUCKET_SIZE*sizeof(T*));
    xpos = 0; ypos = 0;
}
template <class T>
void Hash<T>::insert_element(T element)
{
    //location(xpos, ypos)
    str2upper(element.name);
    int pos = SDBMHash(element.name);
    T* ptr = Array[pos];
    T* p = new T;
    *p = element;
    xpos = pos;
    int counter = 0;
    if (ptr)
    {
        while(ptr->next)
        {
            ptr = ptr->next;
            counter++;
        }
        ptr->next = p;
    }
    else
    {
        Array[pos] = p;
    }
    ypos = counter;
}
template <class T>
T* Hash<T>::search_element(char* key)
{
    str2upper(key);
    int pos = SDBMHash(key);
    T *p = Array[pos];
    int i = 0;
    if (p)
    {
        while(p && strcmp(key, p->name))
        {
            p = p->next;
            i++;
        }
        if (p)
        {
            xpos = pos;
            ypos = i;
            return p;
        }
    }
    return NULL;
}
template <class T>
int Hash<T>::delete_element(const char* key)
{
    //.......
    return -1;
}
// template ins
template class Hash<key_word>;
template class Hash<symbol>;
