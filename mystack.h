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
    T pop();
    int npop(int n);
private:
    int top;
    T ARRAY[STACK_SIZE];
};
#endif // MYSTACK_H
