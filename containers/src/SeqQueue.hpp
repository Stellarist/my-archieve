#pragma once

#include <iostream>
using std::cout, std::endl;

template <typename T, const int DEFAULT_SIZE=10>
class SeqQueue{
private:
    int length;
    int capacity;
    int front;
    T* element;
public:
    SeqQueue(int size=DEFAULT_SIZE)
        : length(0), capacity(size), front(0), element(new T[size]) {}
    ~SeqQueue()
        {delete[] element;}
    T& front_element()
        {return element[front];}
    const T& front_element() const
        {return element[front];}
    T& back_element()
        {return element[(front+length-1)%capacity];}
    const T& back_element() const
        {return element[(front+length-1)%capacity];}
    constexpr bool empty() const
        {return length==0 ? 1 : 0;}
    constexpr bool full() const
        {return length==capacity ? 1 : 0;}
    constexpr int size() const
        {return length;}
    void clear()
        {front=0; length=0;}
    void push(const T& rhs);
    void pop();
    void display();
};

template <typename T, const int DEFAULT_SIZE>
void SeqQueue<T, DEFAULT_SIZE>::push(const T& rhs)
{
    if(full())
        cout<<"queue-overflow"<<endl;
    else
        element[(front+length++)%capacity]=rhs;
}

template <typename T, const int DEFAULT_SIZE>
void SeqQueue<T, DEFAULT_SIZE>::pop()
{
    if(empty())
        cout<<"queue-empty"<<endl;
    else{
        ++front;
        --length;
        if(front>=capacity)
            front-=capacity;
    }
}

template <typename T, const int DEFAULT_SIZE>
void SeqQueue<T, DEFAULT_SIZE>::display()
{
    cout<<"size:  "<<size()<<endl
        <<"capacity  "<<capacity<<endl;
    for(int i=0; i<length; i++)
        cout<<element[(i+length)%capacity];
}

