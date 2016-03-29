#include <iostream>
#include <string.h>
#include "hash.h"
#include "DataStructure.h"
using namespace std;
//Hash function
int DJBHash(const char* s)
{
    long h = 5381;
    while (*s != '\0')
    {
        h = ((h << 5) + h) + *s;
        s ++;
    }
    return h % BUCKET_SIZE;
}

//Hash definition
template <class T>
Hash<T>::Hash()
{
    memset(Array, 0, BUCKET_SIZE*sizeof(T*));
}
template <class T>
void Hash<T>::insert_element(T element)
{
    int pos = DJBHash(element.name);
    T* ptr = Array[pos];
    T* p = new T;
    *p = element;
    if (ptr)
    {
        while(ptr->next)
        {
            ptr = ptr->next;
        }
        ptr->next = p;
    }
    else
    {
        Array[pos] = p;
    }
}
template <class T>
int Hash<T>::search_element(const char* key)
{
    int pos = DJBHash(key);
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
            return pos;
        }
    }
    return -1;
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
