#pragma once

#include <iostream>
using std::ostream;
template <typename T> class SequentList;
template <typename T> ostream& operator<<(ostream&, const SequentList<T>&);
template <typename T> void merge(SequentList<T>&, const SequentList<T>&, const SequentList<T>&);

template <typename T>
class SequentList{
private:
    T* first;
    T* last;
    T* end_of_storage;
public:
    SequentList(): first(nullptr), last(nullptr), end_of_storage(nullptr) {}
    SequentList(int size): first(new T[size]), last(first+size),  end_of_storage(last) {}
    ~SequentList() {delete[] first;}
    SequentList& operator=(const SequentList<T>&);
    T& operator[](int n)
        {return *(first+n);}
    const T& operator[](int n) const
        {return *(first+n);}
    constexpr int size() const
        {return last-first;}
    constexpr int capacity() const
        {return end_of_storage-first;}
    constexpr bool empty() const
        {return last==first  ? 1 : 0;}
    void reserve(unsigned);
    void exchange(SequentList<T>& rhs);
    void push_back(const T&);
    // components of requirements
    constexpr int max() const;
    constexpr int min() const;
    void push_back_inorder(const T&);
    void deleteRange(int, int);
friend ostream& operator<< <>(ostream&, const SequentList<T>&);
friend void merge<>(SequentList<T>&, const SequentList<T>&, const SequentList<T>&);
};

template <typename T>
ostream& operator<<(ostream& out, const SequentList<T>& rhs)
{
    for(T* p=rhs.first; p<rhs.last; ++p)
        out<<*p;
    return out;
}

template <typename T>
inline auto SequentList<T>::operator=(const SequentList<T>& rhs) -> SequentList&
{
    if(this!=&rhs){
        if(capacity()>rhs.size())
            for(int i=0; i<rhs.size(); ++i)
                *(first+i)=rhs[i];
            last=first+rhs.size();
    }else{
        delete[] first;
        first=new T[rhs.size()];
        for(int i=0; i<rhs.size(); i++)
            *(first+i)=rhs[i];
        last=first+rhs.size();
        end_of_storage=first+rhs.size();
    }
    return *this;
}

template <typename T>
inline void SequentList<T>::reserve(unsigned new_capacity)
{
    if(new_capacity<=capacity())
        return;
    else{
        SequentList<T> temp(new_capacity);
        temp=*this;
        temp.exchange(*this);
    }
}

template <typename T>
inline void SequentList<T>::exchange(SequentList<T>& rhs)
{
    T* temp;
    temp=rhs.first;
    rhs.first=first;
    first=temp;
    temp=rhs.last;
    rhs.last=last;
    last=temp;
    temp=rhs.end_of_storage; 
    rhs.end_of_storage=end_of_storage;
    end_of_storage=temp;
}

template <typename T>
void SequentList<T>::push_back(const T& new_element)
{
    if(last==end_of_storage){
        int new_size=size() ? 2*size() :1;
        SequentList temp;
        temp.reserve(new_size);
        temp=*this;
        temp.exchange(*this);
    }
    *last++=new_element;
}

// components of requirements

template <typename T>
constexpr int SequentList<T>::max() const
{
    if(!size())
        return -1;
    T max_value=*first;
    int max_number=0;
    for(int i=0; i<size(); ++i){
        if(*(first+i)>max_value){
            max_value=*(first+i);
            max_number=i;
        }
    }
    return max_number+1;
}

template <typename T>
constexpr int SequentList<T>::min() const
{
    if(!size())
        return -1;
    T min_value=*first;
    int min_number=0;
    for(int i=0; i<size(); ++i){
        if(*(first+i)<min_value){
            min_value=*(first+i);
            min_number=i;
        }
    }
    return min_number+1;
}

template <typename T>
void SequentList<T>::push_back_inorder(const T& rhs)
{
    // expand the capacity
    if(last==end_of_storage){
        int new_size=size() ? 2*size() :1;
        SequentList temp;
        temp.reserve(new_size);
        temp=*this;
        temp.exchange(*this);
    }
    // memorize the elements postion
    T* order=first;
    if(!size()){
        ++last;
        *first=rhs;
    }else{
        T* movement=last-1;
        while(order!=last && *order<=rhs)
            ++order;
        ++last;
        while(movement>=order){
            *(movement+1)=*movement;
            --movement;
        }
        *order=rhs;
    }
}

template <typename T>
void SequentList<T>::deleteRange(int s, int t)
{
    T* p=first;
    int begin=0, end=0;
    // check the bound
    if(*first>=t || *(last-1)<s)
        return;
    else{
        // locate the elements
        for(; (p+begin)&&*(p+begin)<s; ++begin);
        for(; (p+end)&&*(p+end)<t; ++end);
        for(T* r=p+begin,* s=p+end; s<last&&r<p+end; r++,s++)
            *r=*s;
        last-=end-begin;
    }
}

template <typename T>
void merge(SequentList<T>& des, const SequentList<T>& src1, const SequentList<T>& src2)
{
    if(!des.size())
        des.last=des.first;
    des.reserve(src1.size()+src2.size());
    T* p=src1.first, * q=src2.first;
    while(p<src1.last && q<src2.last){
        if(*p<=*q)
            des.push_back(*p++);
        else
            des.push_back(*q++);
    }
    while(p<src1.last)
        des.push_back(*p++);
    while(q<src2.last)
        des.push_back(*q++);
}

