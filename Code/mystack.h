#ifndef MYSTACK_H
#define MYSTACK_H
#define STACK_SIZE 1000
template <class T>
class MyStack
{
public:
    MyStack();
    int is_empty();
    int is_full();
    void push(T element);
    T top_ele();
    T top_ele_by_off(int offset);
    //T top();
    int npop(int n);
    T ARRAY_[STACK_SIZE];
    int top;
private:

};
#endif // MYSTACK_H
