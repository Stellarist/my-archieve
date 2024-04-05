#pragma once

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <algorithm>

template<typename T>
class Vector {
public:
    using value_type             = T;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Vector();
    Vector(size_type n);
    Vector(size_type n, const_reference value);
    Vector(const Vector<T>&);
    template<typename InputIt>
    Vector(InputIt first, InputIt last);
    Vector(iterator, iterator);
    Vector(Vector<T>&&);
    Vector(std::initializer_list<T>);
    Vector& operator=(const Vector<T>&);
    Vector& operator=(Vector<T>&&);
    ~Vector();

    reference at(size_type pos)
        {if(pos>=size()) throw std::range_error("Vector out of range.");
        return *(start+pos);}

    const_reference at(size_type pos) const
        {if(pos>=size()) throw std::range_error("Vector out of range.");
        return *(start+pos);}

    reference operator[](size_type pos) 
        {return *(start+pos);}

    const_reference operator[] (size_type pos) const
        {return *(start+pos);}

    pointer data()
        {return start;}
    
    const_pointer data() const
        {return start;}

    reference front()
        {assert(!empty() && "Vector::front() called on an empty Vector");
        return *start;}

    const_reference front() const
        {assert(!empty() && "Vector::front() called on an empty Vector");
        return *start;}

    reference back()
        {assert(!empty() && "Vector::back() called on an empty Vector");
        return *(finish-1);}

    const_reference back() const
        {assert(!empty() && "Vector::front() called on an empty Vector");
        return *(finish-1);}

    iterator begin()
        {return start;}

    const_iterator begin() const
        {return start;}
    
    const_iterator cbegin() const
        {return start;}

    iterator end()
        {return finish;}

    const_iterator end() const
        {return finish;}

    const_iterator cend() const
        {return finish;}

    reverse_iterator rbegin()
        {return reverse_iterator(finish);}
    
    const_reverse_iterator rbegin() const
        {return const_reverse_iterator(finish);}
    
    const_reverse_iterator crbegin() const
        {return const_reverse_iterator(finish);}
    
    reverse_iterator rend()
        {return reverse_iterator(start);}
    
    const_reverse_iterator rend() const
        {return const_reverse_iterator(start);}

    const_reverse_iterator crend() const
        {return const_reverse_iterator(start);}
    
    bool empty() const
        {return start==finish;}

    size_type size() const
        {return static_cast<size_type>(finish-start);}
    
    size_type capacity() const
        {return static_cast<size_type>(end_of_storage-start);}

    iterator insert(const_iterator, const_reference);
    iterator insert(const_iterator, value_type&&);
    iterator insert(const_iterator, size_type, const_reference);
    template <typename InputIt>
    iterator insert(const_iterator, InputIt, InputIt);
    iterator insert(const_iterator, std::initializer_list<T>);
    iterator erase(const_iterator);
    iterator erase(const_iterator, const_iterator);
    void push_back(const_reference);
    void push_back(value_type&&);
    void pop_back();
    void clear();
    void swap(Vector<T>&);
    void resize(size_type);
    void reserve(size_type);
    void shrink_to_fit();

private:
    iterator start;
    iterator finish;
    iterator end_of_storage;
};

template<typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return lhs.size()==rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs==rhs);
}

template <typename T>
inline Vector <T>::Vector()
: start(nullptr), finish(nullptr), end_of_storage(nullptr) {}

template <typename T>
inline Vector<T>::Vector(size_type n)
: start(new T[n]()), finish(start+n), end_of_storage(finish) 
{
    std::uninitialized_fill_n(start, n, T());
}

template <typename T>
inline Vector<T>::Vector(size_type n, const_reference value)
: start(new T[n]), finish(start+n), end_of_storage(finish) 
{
    std::uninitialized_fill_n(start, n, value);
}

template <typename T>
inline Vector<T>::Vector(const Vector<T>& rhs)
: start(new T[rhs.size()]), finish(start+rhs.size()), end_of_storage(finish)
{
    std::uninitialized_copy(rhs.begin(), rhs.end(), start);
}

template <typename T>
template <typename InputIt>
inline Vector<T>::Vector(InputIt first, InputIt last)
: start(new T[last-first]), finish(start+(last-first)), end_of_storage(finish)
{
    std::uninitialized_copy(first, last, start);
}

template <typename T>
inline Vector<T>::Vector(Vector<T>&& rhs)
: start(rhs.start), finish(rhs.finish), end_of_storage(rhs.end_of_storage)
{
    rhs.start=rhs.finish=rhs.end_of_storage=nullptr;
}

template <typename T>
inline Vector<T>::Vector(std::initializer_list<T> il)
: start(new T[il.size()]), finish(start+il.size()), end_of_storage(finish)
{
    std::uninitialized_copy(il.begin(), il.end(), start);
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
{
    if(this!=&rhs){
        if(capacity()>=rhs.size()){
            for(size_type i=0; i<rhs.size(); i++)
                *(start+i)=rhs[i];
            finish=start+rhs.size();
        }else{
            delete[] start;
            start=new T[rhs.size()];
            for(size_type i=0; i<rhs.size(); i++)
                *(start+i)=rhs[i];
            finish=start+rhs.size();
            end_of_storage=start+rhs.size();
        }
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator=(Vector<T>&& rhs)
{
    if(this!=&rhs){
        delete[] start;
        start=rhs.start;
        finish=rhs.finish;
        end_of_storage=rhs.end_of_storage;
        rhs.start=rhs.finish=rhs.end_of_storage=nullptr;
    }
    return *this;
}

template <typename T>
inline Vector<T>::~Vector()
{
    delete[] start;
}

template<typename T>
inline auto Vector<T>::insert(const_iterator pos, const_reference value) -> iterator
{
    return insert(pos,1,value);
}

template<typename T>
inline auto Vector<T>::insert(const_iterator pos, value_type&& value) -> iterator
{
    return insert(pos,1,std::move(value));
}

template<typename T>
inline auto Vector<T>::insert(const_iterator pos, size_type num, const_reference value) -> iterator
{
    assert(pos<=finish && "Vector::insert(iterator, size_type, const_reference) called with a non-dereferenceable iterator");
    size_type offset=pos-start;
    if(size()+num>capacity())
        reserve(size()+num);
    std::move(start+offset, finish, start+offset+num);
    std::uninitialized_fill_n(start+offset, num, value);
    finish+=num;
    return start+offset;
}

template<typename T>
template<typename InputIt>
inline auto Vector<T>::insert(const_iterator pos, InputIt first, InputIt last) -> iterator
{
    assert(pos<=finish && "Vector::insert(iterator, iterator, iterator) called with a non-dereferenceable iterator");
    size_type offset=pos-start;
    size_type num=last-first;
    if(size()+num>capacity())
        reserve(size()+num);
    std::move(start+offset, finish, start+offset+num);
    std::uninitialized_copy(first, last, start+offset);
    finish+=num;
    return start+offset;
}

template<typename T>
inline auto Vector<T>::insert(const_iterator pos, std::initializer_list<T> il) -> iterator
{
    assert(pos<=finish && "Vector::insert(iterator, std::initializer_list) called with a non-dereferenceable iterator");
    size_type offset=pos-start;
    size_type num=il.size();
    if(size()+num>capacity())
        reserve(size()+num);
    std::move(start+offset, finish, start+offset+num);
    std::uninitialized_copy(il.begin(), il.end(), start+offset);
    finish+=num;
    return start+offset;
}

template<typename T>
inline auto Vector<T>::erase(const_iterator pos) -> iterator
{
    return erase(pos, pos+1);
}

template<typename T>
inline auto Vector<T>::erase(const_iterator first, const_iterator last) -> iterator
{
    assert(first<=last && "Vector::erase(const_iterator, const_iterator) called with invalid range");

    size_type offset=first-start;
    std::destroy(first, last);
    std::move(const_cast<iterator>(last), finish, const_cast<iterator>(first));
    finish=finish-(last-first);
    return start+offset;
} 

template<typename T>
inline void Vector<T>::push_back(const_reference new_element)
{
    if(finish==end_of_storage){
        size_type new_size= size() ? 2*size() : 1;
        reserve(new_size);
    }
    *finish++=new_element;
}

template<typename T>
inline void Vector<T>::push_back(value_type&& new_element)
{
    if(finish==end_of_storage){
        size_type new_size= size() ? 2*size() : 1;
        reserve(new_size);
    }
    *finish++=std::move(new_element);
}

template<typename T>
inline void Vector<T>::pop_back()
{
    assert(!empty() && "Vector::pop_back called on an empty vector");
    finish--;
}

template<typename T>
inline void Vector<T>::clear()
{
    std::destroy(start, finish);
    finish=start;
}

template<typename T>
inline void Vector<T>::swap(Vector<T>& rhs)
{
    std::swap(start, rhs.start);
    std::swap(finish, rhs.finish);
    std::swap(end_of_storage, rhs.end_of_storage);
}

template<typename T>
inline void Vector<T>::resize(size_type new_size)
{
    if(new_size>=capacity()){
        reserve(new_size);
        finish=start+new_size;
    }else
        finish=start+new_size;
}

template<typename T>
inline void Vector<T>::reserve(size_type new_capacity)
{
    if(new_capacity>capacity()) {
        Vector temp(new_capacity);
        temp.finish=temp.start+size();
        temp.swap(*this);
        std::move(temp.start, temp.finish, start);
    }
}

template<typename T>
inline void Vector<T>::shrink_to_fit()
{
    if(size()<capacity()) {
        Vector temp(*this);
        temp.swap(*this);
    }
}
