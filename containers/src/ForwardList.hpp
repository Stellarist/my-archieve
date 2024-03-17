#pragma once

#include <iostream>
using std::ostream;
template <typename T> class Node;
template <typename T> class ForwardList;
template <typename T> ostream& operator<<(ostream&, const Node<T>&);
template <typename T> ostream& operator<<(ostream&, const ForwardList<T>&); 

template <typename T>
class Node{
private:
    T data;
    Node<T>* next;
public:
    Node()
        :next(nullptr) {}
    Node(const T& content)
        :data(content), next(nullptr) {}
    Node(const Node<T>& rhs)
        :data(rhs.data), next(nullptr) {}
    ~Node()=default;
    T& operator*(Node<T>*& p)
        {return p->data;}
friend ForwardList<T>; 
friend ostream& operator<< <>(ostream&, const Node<T>&);
friend ostream& operator<< <>(ostream&, const ForwardList<T>&);
};

template <typename T>
class ForwardList{
private:
    int list_size;
    Node<T>* head;
    ForwardList<T>& operator=(const ForwardList<T>& rhs)
        {return *this;}
public:
    ForwardList()
        :list_size(0), head(new Node<T>) {}
    ~ForwardList()
        {delete head;}
    ForwardList<T>& operator[](int);
    void push_back(const T&);
    void push_back_inorder(const T&);
    void reversal();
    void merge(ForwardList<T>& src1, ForwardList<T>& src2);
friend ostream& operator<< <>(ostream&, const ForwardList<T>&);
};

template <typename T>
ForwardList<T>& ForwardList<T>::operator[](int n)
{
    Node<T>* p=head;
    for(int i=0; i<n; i++)
        p=p->next;
    return *p;
}

// overload output function
template <typename T>
ostream& operator<<(ostream& out, const Node<T>& rhs)
{
    out<<rhs.data;
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, const ForwardList<T>& rhs)
{
    Node<T>* p=rhs.head->next;
    while(p){
        out<<*p;
        p=p->next;
    }
    return out;
}

template <typename T>
void ForwardList<T>::push_back(const T& rhs)
{
    Node<T>* p=head;
    while(p->next)
        p=p->next;
    Node<T>* temp=new Node<T>(rhs);
    p->next=temp;
    temp->next=nullptr;
    ++list_size;
}

// components of requirements
template <typename T>
void ForwardList<T>::push_back_inorder(const T& rhs)
{
    Node<T>* p=head;
    Node<T>* temp=new Node<T>(rhs);
    while(p->next && p->next->data<=rhs)
        p=p->next;
    temp->next=p->next;
    p->next=temp;
    ++list_size;
}

template <typename T>
void ForwardList<T>::reversal()
{
    if(list_size==0 || list_size==1)
        return;
    else if(list_size==2){
        head->next->next->next=head->next;
        head->next=head->next->next;
        head->next->next->next=nullptr;
    }else{
        Node<T>* p,* q,* r;
        p=head->next;
        q=p->next;
        r=q->next;
        p->next=nullptr;
        while(r){
            q->next=p;
            p=q;
            q=r;
            r=r->next;
        }
        q->next=p;
        head->next=q;
    }
}

template <typename T>
void ForwardList<T>::merge(ForwardList<T>& src1, ForwardList<T>& src2)
{
    Node<T>* p=src1.head->next;
    Node<T>* q=src2.head->next;
    while(p&&q){
        if(p->data<=q->data){
            push_back(p->data);
            p=p->next;
        }else{
            push_back(q->data);
            q=q->next;
        }
    }
    while(p){
        push_back(p->data);
        p=p->next;
    }
    while(q){
        push_back(q->data);
        q=q->next;
    }
}

