#pragma once

#include <iostream>
#include <vector>

template <typename T> class MinHeap;
template <typename T> class MaxHeap;
template <typename T> std::ostream& operator<<(std::ostream&, const MinHeap<T>&);
template <typename T> std::ostream& operator<<(std::ostream&, const MaxHeap<T>&);

template <typename T>
class MinHeap{
private:
    int size;
    int capacity;
    std::vector<T> element;
public:
    MinHeap(T*, int);
friend std::ostream& operator<< <>(std::ostream&, const MinHeap&);
};

template <typename T>
class MaxHeap{
private:
    int size;
    int capacity;
    std::vector<T> element;
public:
    MaxHeap()
        : size(0), capacity(1), element(1,T()) {}
    MaxHeap(int new_size)
        : size(0), capacity(new_size+1), element(new_size+1) {}
    MaxHeap(T*, int);
    ~MaxHeap()=default;
    constexpr int getsize() const
        {return size;}
    constexpr bool empty() const
        {return size==1;}
    constexpr bool full() const
        {return size==capacity-1;}
    T& top()
        {return element.at(1);}
    void clear()
        {size=0;}
    void push(const T&);
    void pop();
friend std::ostream& operator<< <>(std::ostream&, const MaxHeap&);
};

template <typename T>
MinHeap<T>::MinHeap(T* array, int size)
    :size(0), capacity(1)
{
    element.push_back(T());
    auto push=[&](const T& value) -> void {
        int i;
        if(size==capacity-1){
            capacity=capacity ? 2*capacity : 1;
            element.resize(capacity);
        }
        for(i=++this->size; element[i/2]>value && i/2; i/=2)
            element[i]=element[i/2];
        element[i]=value;
    };
    for(int i=0; i<size; i++)
        push(array[i]);
}

template <typename T>
MaxHeap<T>::MaxHeap(T* array, int size)
    :size(0), capacity(1)
{
    element.push_back(T());
    for(int i=0; i<size; i++)
        push(array[i]);
}

template <typename T>
auto MaxHeap<T>::push(const T& value) -> void
{
    int i;
    if(full()){
        capacity=capacity ? 2*capacity : 1;
        element.resize(capacity);
    }
    for(i=++size; element[i/2]<value && i/2; i/=2)
        element[i]=element[i/2];
    element[i]=value;
}

template <typename T>
auto MaxHeap<T>::pop() -> void
{
    if(empty())
        return;
    int i, child;
    T last=element[size--];
    for(i=1, child=2; 2*i<=size; i=child){
        child=2*i;
        if(child!=size && element[child+1]>element[child])
            child++;
        if(last<element[child])
            element[i]=element[child];
        else
            break;
    }
    element[i]=last;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const MinHeap<T>& rhs)
{
    for(int i=1; i<=rhs.size; i++)
        out<<rhs.element[i]<<" ";
    out<<std::endl;
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const MaxHeap<T>& rhs)
{
    for(int i=1; i<=rhs.size; i++)
        out<<rhs.element[i]<<" ";
    out<<std::endl;
    return out;
}
