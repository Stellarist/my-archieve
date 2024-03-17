#pragma once

template <typename T> class Vector;

template <typename T>
class Vector{
private:
    using reference = T&;
    using const_reference = const T&;

public:
    // can be used as Vector<T>::(const_)iterator
    // out of class (T should be replaced)
    using iterator = T*;
    using const_iterator = const T*;

private:
    iterator start;
    iterator finish;
    iterator end_of_storage;

public:
    // Ctor, dtor and assignment
    Vector();
    Vector(int n);
    Vector(int n, const T& value);
    Vector(const Vector<T>&);
    Vector(T*, T*);
    Vector& operator=(const Vector<T>&);
    ~Vector();

    // Element access
    reference at(unsigned pos)
        {if(pos>=0 && pos<size()) return (*this)[pos];
        else throw("out of range");}
    const_reference at(unsigned pos) const
        {if(pos>=0 && pos<size()) return (*this)[pos];
        else throw("out of range");}
    reference operator[](unsigned pos)
        {return *(start+pos);}
    const_reference operator[] (unsigned pos) const
        {return *(start+pos);}
    reference front()
        {if(empty()) throw("out-of-range"); return *start;}
    const_reference front() const
        {if(empty()) throw("out-of-range"); return *start;}
    reference back()
        {if(empty()) throw("out-of-range"); return *(finish-1);}
    const_reference back() const
        {if(empty()) throw("out-of-range"); return *(finish-1);}

    // Iterators
    iterator begin()
        {return start;}
    const_iterator begin() const
        {return start;}
    iterator end()
        {return finish;}
    const_iterator end() const
        {return finish;}

    // Capacity
    bool empty() const
        {return start==finish ? 1 : 0;}
    unsigned size() const
        {return finish-start;}
    unsigned capacity() const
        {return end_of_storage-start;}
    void reserve(unsigned new_capacity)
        {if(new_capacity<=capacity()) return;
         else {Vector<T> temp(new_capacity); temp=*this; temp.swap(*this);}}
    void shrink_to_fit()
        {Vector<T>(*this).swap(*this);}

    // Modifiers
    void clear()
        {finish=start;}
    void insert(iterator, const T&);
    void insert(iterator, unsigned, const T&);
    void erase(iterator);
    void erase(iterator, iterator);
    void push_back(const T&);
    void pop_back();
    void resize(unsigned);
    void swap(Vector<T>&);
};

// non-member functions
// overloaded operators declaration
template<typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if(lhs.size()!=rhs.size())
        return 0;
    else
        for(int i=0; i<lhs.size()&&i<rhs.size(); i++)
            if(lhs[i]!=rhs[i])
                return 0;
    return 1;
}

template<typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs==rhs);
}

// member functions
// ctor with no initial menber
template <typename T>
inline Vector <T>::Vector()
: start(nullptr), finish(nullptr), end_of_storage(nullptr) {}

// ctor with specific size
template <typename T>
inline Vector<T>::Vector(int n)
: start(new T[n]()), finish(start+n), end_of_storage(finish) {}

template <typename T>
inline Vector<T>::Vector(int n, const T& value)
: start(new T[n]), finish(start+n), end_of_storage(finish) 
{
    for(T* p=start; p!=finish; p++)
        *p=value; 
}

template <typename T>
inline Vector<T>::Vector(const Vector<T>& rhs)
: start(new T[rhs.size()]), finish(start+rhs.size()), end_of_storage(finish)
{
    for(int i=0;i<rhs.size();i++)
        *(start+i)=rhs[i]; 
}

template <typename T>
inline Vector<T>::Vector(T* first, T* last)
: start(new T[last-first]), finish(start+(last-first)), end_of_storage(finish)
{
    for(int i=0; i<last-first; i++)
        *(start+i)=*(first+i);
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
{
    if(this!=&rhs){
        if(capacity()>=rhs.size()){
            for(int i=0; i<rhs.size(); i++)
                *(start+i)=rhs[i];
            finish=start+rhs.size();
        }else{
            delete[] start;
            start=new T[rhs.size()];
            for(int i=0; i<rhs.size(); i++)
                *(start+i)=rhs[i];
            finish=start+rhs.size();
            end_of_storage=start+rhs.size();
        }
    }
    return *this;
}

template <typename T>
inline Vector<T>::~Vector()
{
    delete[] start;
}

template<typename T>
inline void Vector<T>::insert(T* pos, const T& value)
{
    insert(pos,1,value);
}

template<typename T>
inline void Vector<T>::insert(T* pos, unsigned num, const T& value)
{
    int offset=pos-start;
    int move=finish-pos+1;
    at(offset);          // check the bound
    if(size()+num>capacity())
        reserve(size()+num);
    finish+=num;
    for(int i=1; i<=move; i++)
        *(finish-i)=*(finish-num-i);
    for(T* p=start+offset; p<start+offset+num; p++)
        *p=value; 
}

template<typename T>
inline void Vector<T>::erase(T* pos)
{
    if(pos>=finish || pos<start)
        throw("out-of-range");
    else{
        for(T* p=pos; p<finish-1; p++)
            *p=*(p+1);
        --finish;
    }
}

template<typename T>
inline void Vector<T>::erase(T* first, T* last)
{
    if(first>last)
        throw("range-fault");
    else if(first<start || last>finish)
        throw("out-of-range");
    else{
        int new_size = size() - (last - first);
        for (int i = 0; i < finish-last; i++)
            *(first+i)=*(last+i);
        finish = start + new_size;
    }
} 

// if needed, reallocate 2 times
template<typename T>
inline void Vector<T>::push_back(const T& new_element)
{
    if(finish==end_of_storage){
        int new_size= size() ? 2*size() : 1;
        Vector temp;
        temp.reserve(new_size);
        temp=*this;
        temp.swap(*this);
    }
    *finish++=new_element;
}

template<typename T>
inline void Vector<T>::pop_back()
{
    *(finish-1)=T();
    finish--;
}

template<typename T>
inline void Vector<T>::resize(unsigned new_size)
{
    if(new_size>=capacity()){
        reserve(new_size);
        for(T* p=finish; p<=end_of_storage; p++)
            *p=T();
        finish=start+new_size;
    }
    else{
        Vector<T> temp(new_size);
        for(int i=0; i<new_size; i++)
            temp[i]=*(start+i);
        temp.swap(*this);
    }
}

// swap(only swap pointer)
template<typename T>
inline void Vector<T>::swap(Vector<T>& rhs)
{
    T* temp;
    temp=rhs.start;
    rhs.start=start;
    start=temp;
    temp=rhs.finish;
    rhs.finish=finish;
    finish=temp;
    temp=rhs.end_of_storage; 
    rhs.end_of_storage=end_of_storage;
    end_of_storage=temp;
}

