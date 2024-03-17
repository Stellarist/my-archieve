#pragma once

#include <iostream>
using std::cout, std::endl;
template <typename T> class ListQueue;
template <typename T> class ListQueue2;

template <typename T>
class QNode{
private:
    T data;
    QNode<T>* next;
public:
    QNode()
        : next(nullptr) {}
    QNode(const QNode& rhs)
        : data(rhs.data), next(nullptr) {} 
    ~QNode()=default;
friend ListQueue<T>;
};

template <typename T>
class ListQueue{
private:
    QNode<T>* front, * rear;
public:
    ListQueue()
        : front(new QNode<T>), rear(front) {}
    ~ListQueue()
        {clear(); delete front;}
    int size() const;
    constexpr bool empty() const
        {return front==rear ? 1 : 0;}
    T& front_element()
        {return front->next->data;}
    const T& front_element() const
        {return front->next->data;}
    T& back_element()
        {return rear->data;}
    const T& back_element() const
        {return rear->data;}
    void clear();
    void push(const T&);
    void pop();
    void display();
};

template <typename T>
int ListQueue<T>::size() const
{
    int length=0;
    for(auto p=front->next; p; p=p->next)
        length++;
    return length;
}

template <typename T>
void ListQueue<T>::clear()
{
    QNode<T>* p=front->next;
    while (p){
        front->next=p->next;
        delete p;
        p=front->next;
    }
    rear=front;
}

template <typename T>
void ListQueue<T>::push(const T& rhs)
{
    QNode<T>* temp=new QNode<T>;
    temp->data=rhs;
    rear->next=temp;
    rear=temp;
    rear->next=nullptr;
}

template <typename T>
void ListQueue<T>::pop()
{
    if(empty())
        cout<<"queue-empty"<<endl;
    else{
        QNode<T>* temp=front->next;
        front->next=temp->next;
        if(size()==1)
            rear=front;
        delete temp;
    }
}

template <typename T>
void ListQueue<T>::display()
{
    cout<<"size:  "<<size()<<endl;
    for(auto p=front->next; p; p=p->next)
        cout<<p->data; 
}

template <typename T>
class QNode2{
private:
    T data;
    QNode2<T>* next;
public:
    QNode2()
        : next(nullptr) {}
    QNode2(const QNode2& rhs)
        : data(rhs.data), next(nullptr) {} 
    ~QNode2()=default;
friend ListQueue2<T>;
};


template <typename T>
class ListQueue2{
private:
    QNode2<T>* rear;
public:
    ListQueue2()
        : rear(nullptr) {}
    ~ListQueue2()
        {clear(); delete rear;}
    int size() const;
    constexpr bool empty() const
        {return !rear ? 1 : 0;}
    T& front_element()
        {return rear->next->data;}
    const T& front_element() const
        {return rear->next->data;}
    T& back_element()
        {return rear->data;}
    const T& back_element() const
        {return rear->data;}
    void clear();
    void push(const T&);
    void pop();
    void display();
};

template <typename T>
int ListQueue2<T>::size() const
{
    int length=0;
    if(empty())
        length=0;
    else if(rear->next==rear)
        length=1;
    else{
        for(auto p=rear->next; p!=rear; p=p->next)
            length++;
        length++;
    }
    return length;
}

template <typename T>
void ListQueue2<T>::clear()
{
    if(empty())
        return;
    int temp_size=size();
    for(int i=0; i<temp_size; i++)
        pop();
}

template <typename T>
void ListQueue2<T>::push(const T& rhs)
{
    if(empty()){
        rear=new QNode2<T>;
        rear->data=rhs;
        rear->next=rear;
    }else{
        QNode2<T>* temp=new QNode2<T>;
        temp->data=rhs;
        temp->next=rear->next;
        rear->next=temp;
        rear=temp;
    }
}

template <typename T>
void ListQueue2<T>::pop()
{
    if(empty())
        cout<<"queue-empty"<<endl;
    else if(size()==1){
            delete rear;
            rear=nullptr;
    }else{
        QNode2<T>* temp=rear->next;
        rear->next=temp->next;
        delete temp;
    }
}

template <typename T>
void ListQueue2<T>::display()
{
    cout<<"size:  "<<size()<<endl;
    if(empty())
        return;
    QNode2<T>* p=rear->next;
    for(int i=0; i<size(); i++){
        cout<<p->data;
        p=p->next;
    }
}

