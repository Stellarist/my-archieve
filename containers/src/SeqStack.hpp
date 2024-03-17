#pragma once

#include <iostream>
using std::cout, std::endl;

template <typename T, const int DEFAULT_SIZE=10>
class SeqStack{
private:
    int capacity;
    int top_of_stack;
    T* element;
public:
    SeqStack(int size=DEFAULT_SIZE)
        :capacity(size), top_of_stack(-1), element(new T[size]) {}
    SeqStack(const SeqStack&);
    ~SeqStack()
        {delete[] element;}
    SeqStack& operator=(const SeqStack&);
    constexpr int size() const
        {return top_of_stack+1;}
    constexpr bool full() const
        {return top_of_stack==capacity-1 ? 1 : 0;}
    constexpr bool empty() const
        {return top_of_stack==-1 ? 1 : 0;}
    T& top()
        {return *(element+top_of_stack);}
    const T& top() const
        {return *(element+top_of_stack);}
    void clear()
        {top_of_stack=-1;}
    void push(const T&);
    void push2(const T&);
    void pop();
    void stackfull();
    void display();
};

template <typename T, const int DEFAULT_SIZE>
SeqStack<T, DEFAULT_SIZE>::SeqStack(const SeqStack& rhs)
    : element(new T[rhs.capacity()]), top_of_stack(rhs.top_of_stack), capacity(rhs.capacity)
{
    for(int i=0; i<=rhs.top_of_stack; i++)
        element[i]=rhs.element[i];
}

template <typename T, const int DEFAULT_SIZE>
auto SeqStack<T, DEFAULT_SIZE>::operator=(const SeqStack& rhs) -> SeqStack&
{
    if(this!=&rhs){
        if(capacity<rhs.size()){
            delete[] element;
            element=new T[rhs.capacity()];
        }
        top_of_stack=rhs.top_of_stack;
        capacity=rhs.capacity;
        for(int i=0; i<=rhs.top_of_stack; i++)
            element[i]=rhs.element[i];
    }
    return *this;
}

template <typename T, const int DEFAULT_SIZE>
void SeqStack<T, DEFAULT_SIZE>::push(const T& rhs)
{
    if(full())
        cout<<"stack-overflow"<<endl;
    else
        element[++top_of_stack]=rhs; 
}

template <typename T, const int DEFAULT_SIZE>
void SeqStack<T, DEFAULT_SIZE>::pop()
{
    if(empty())
        cout<<"stack-empty"<<endl;
    else
        --top_of_stack;
}

template <typename T, const int DEFAULT_SIZE>
void SeqStack<T, DEFAULT_SIZE>::push2(const T& rhs)
{
    if(full()){
        stackfull();
        push(rhs);
    }
    else
        element[++top_of_stack]=rhs; 
}

template <typename T, const int DEFAULT_SIZE>
void SeqStack<T, DEFAULT_SIZE>::stackfull()
{
    SeqStack<T, DEFAULT_SIZE> temp(2*size());
    for(int i=0; i<size(); i++)
        temp.element[i]=element[i];
    capacity=temp.capacity;
    T* temp_t=temp.element;
    temp.element=element;
    element=temp_t;
}

template <typename T, const int DEFAULT_SIZE>
void SeqStack<T, DEFAULT_SIZE>::display()
{
    cout<<"size:  "<<size()<<endl
        <<"capacity  "<<capacity<<endl;
    for(int i=0; i<=top_of_stack; i++)
        cout<<*(element+i);
}

