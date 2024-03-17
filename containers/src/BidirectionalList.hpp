#pragma once

#include <iostream>
using std::ostream;
template <typename T> class BNode;
template <typename T> class BidirectionalList;
template <typename T> ostream& operator<<(ostream&, const BNode<T>&);
template <typename T> ostream& operator<<(ostream&, const BidirectionalList<T>&); 

template <typename T>
class BNode{
private:
    T data;
    BNode<T>* prev, * next;
public:
    BNode()
        :prev(nullptr), next(nullptr) {}
    BNode(const T& content)
        :data(content), prev(nullptr), next(nullptr) {}
    BNode(const BNode<T>& rhs)
        :data(rhs.data), prev(nullptr), next(nullptr) {}
    ~BNode()=default;
    BNode<T>* operator*(BNode<T>*& p)
        {return p->data;}
friend BidirectionalList<T>; 
friend ostream& operator<< <>(ostream&, const BNode<T>&);
friend ostream& operator<< <>(ostream&, const BidirectionalList<T>&);
};

template <typename T>
class BidirectionalList{
private:
    int list_size;
    BNode<T>* head;
    BidirectionalList<T>& operator=(const BidirectionalList<T>& rhs) {return *this;}
public:
    BidirectionalList()
        :list_size(0), head(new BNode<T>) {head->next=head;}
    ~BidirectionalList()
        {delete head;}
    BidirectionalList<T>& operator[](int);
    void push_back(const T&);
    void push_back_inreverse(const T&);
    void swap();
friend ostream& operator<< <>(ostream&, const BidirectionalList<T>&);
};

template <typename T>
BidirectionalList<T>& BidirectionalList<T>::operator[](int n)
{
    BNode<T>* p=head;
    for(int i=0; i<n; i++)
        p=p->next;
    return *p;
}

// overload output function
template <typename T>
ostream& operator<<(ostream& out, const BNode<T>& rhs)
{
    out<<rhs.data;
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, const BidirectionalList<T>& rhs)
{
    BNode<T>* p=rhs.head->next;
    while(p!=rhs.head){
        out<<*p;
        p=p->next;
    }
    return out;
}

template <typename T>
void BidirectionalList<T>::push_back(const T& rhs)
{
    BNode<T>* p=head;
    while(p->next!=head)
        p=p->next;
    BNode<T>* temp=new BNode<T>(rhs);
    p->next=temp;
    temp->next=head;
    temp->prev=p;
    head->prev=temp;
    ++list_size;
}

template <typename T>
void BidirectionalList<T>::push_back_inreverse(const T& rhs)
{
    BNode<T>* p=head;
    BNode<T>* temp=new BNode<T>(rhs);
    while(p->next!=head && p->next->data>=rhs)
        p=p->next;
    temp->next=p->next;
    temp->prev=p;
    p->next->prev=temp;
    p->next=temp;
    ++list_size;
}

template <typename T>
void BidirectionalList<T>::swap()
{
    if(list_size<2)
        return;
    BNode<T>* max_ptr=head->next;
    BNode<T>* min_ptr=head->next;
    auto max=[this, &max_ptr] () -> BNode<T>*&{
        for(BNode<T>* p=head->next; p->next!=head; p=p->next)
            if(p->data>max_ptr->data)
                max_ptr=p; 
        return max_ptr;
    };
    auto min=[this, &min_ptr] () -> BNode<T>*&{
        for(BNode<T>* p=head->next; p->next!=head; p=p->next)
            if(p->data<min_ptr->data)
                min_ptr=p;
        return min_ptr;
    };
    max();
    min();
    BNode<T>* max_prev=max_ptr->prev;
    BNode<T>* max_next=max_ptr->next;
    BNode<T>* min_prev=min_ptr->prev;
    BNode<T>* min_next=min_ptr->next;
    max_prev->next=min_ptr;
    max_next->prev=min_ptr;
    min_ptr->prev=max_prev;
    min_ptr->next=max_next;
    min_prev->next=max_ptr;
    min_next->prev=max_ptr;
    max_ptr->prev=min_prev;
    max_ptr->next=min_next;
}
