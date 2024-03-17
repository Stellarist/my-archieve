#pragma once

template <typename T>
class Deque{
private:
    using iterator=T*;
    using const_iterator=const T*;
    using reference=T&;
    using const_reference=const T&;
private:
    iterator begin_of_storage;
    iterator end_of_storage;
    iterator start;
    iterator finish;
    void reallocate();
public:
    Deque()
        : begin_of_storage(nullptr), end_of_storage(nullptr), start(nullptr), finish(nullptr) {}
    Deque(const Deque&);
    ~Deque()
        {delete[] begin_of_storage;}
    Deque& operator=(const Deque<T>&);

    // iterators
    iterator begin()
        {return start;}
    const_iterator begin() const
        {return start;}
    iterator end()
        {return finish;}
    const_iterator end() const
        {return finish;}

    // element access
    reference operator[](int pos)                           // no bound checked
        {return *(start+pos);} 
    const_reference operator[](int pos) const       // no bound checked
        {return *(start+pos);} 
    reference front()
        {return *start;}
    const_reference front() const
        {return *start;}
    reference back()
        {return *finish;}
    const_reference back() const
        {return *finish;}

    // capacity
    constexpr bool empty() const
        {return start==finish;}
    constexpr int size() const
        {return finish-start;}
    constexpr int capacity() const
        {return end_of_storage-begin_of_storage;}
    
    // modifiers
    void clear()
        {finish=start;}
    void push_back(const T&);
    void pop_back();
    void push_front(const T&);
    void pop_front();
    void swap(Deque<T>&); 
};

template <typename T>
void Deque<T>::reallocate()
{
    Deque<T> temp;
    int new_capacity=size()>2 ? 2*size() : 4;
    temp.begin_of_storage=new T[new_capacity];
    temp.end_of_storage=temp.begin_of_storage+new_capacity;
    temp.start=temp.begin_of_storage+(new_capacity-size())/2;
    temp.finish=temp.start+size();
    for(iterator it=temp.begin(), it2=begin(); it2!=end(); ++it, ++it2)
        *it=*it2;
    swap(temp);
}

template <typename T>
Deque<T>::Deque(const Deque<T>& rhs)
: begin_of_storage(new T[rhs.capacity()]), end_of_storage(begin_of_storage+rhs.capacity()),
    start(begin_of_storage+(rhs.start-rhs.begin_of_storage)), finish(start+rhs.size()) 
{
    for(iterator it=begin(), it2=rhs.begin(); it2!=rhs.end(); ++it, ++it2)
        *it=*it2;
}

template <typename T>
auto Deque<T>::operator=(const Deque<T>& rhs) -> Deque&
{
    if(this!=&rhs){
        delete[] begin_of_storage;
        begin_of_storage=new T[rhs.capacity];
        end_of_storage=begin_of_storage+rhs.capacity();
        start=begin_of_storage+(rhs.start-rhs.begin_of_storage);
        finish=start+rhs.size();
        for(iterator it=begin(), it2=rhs.begin(); it2!=rhs.end(); ++it, ++it2)
            *it=*it2;
    }
    return *this;
}

template <typename T>
void Deque<T>::push_back(const T& content)
{
    if(finish==end_of_storage)
        reallocate();
    *finish++=content;
}

template <typename T>
void Deque<T>::pop_back()
{
    if(empty())
        throw("deque-empty");
    else
        finish--;
}

template <typename T>
void Deque<T>::push_front(const T& content)
{
    if(start==begin_of_storage)
        reallocate();
    *--start=content;
}

template <typename T>
void Deque<T>::pop_front()
{
    if(empty())
        throw("deque-empty");
    else
        ++start;
}

template <typename T>
void Deque<T>::swap(Deque<T>& rhs)
{
    auto swap=[] (T*& a, T*& b) -> void {
        T* temp=b;
        b=a;
        a=temp;
    };
    swap(start, rhs.start);
    swap(finish, rhs.finish);
    swap(begin_of_storage, rhs.begin_of_storage);
    swap(end_of_storage, rhs.end_of_storage);
}

