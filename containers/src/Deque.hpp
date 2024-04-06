#pragma once

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <deque>

template <typename T> class Deque;
template <typename T> class DequeIterator;

template <typename T>
class DequeIterator {
public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = T;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;
    using self              = DequeIterator<T>;
    using map_pointer       = pointer*;

    DequeIterator(): ptr{}, map_pointer{} {}
    DequeIterator(map_pointer mp, pointer p): map_pointer(mp), ptr(p) {}
    DequeIterator(const DequeIterator& rhs): ptr(rhs.ptr), map_pointer(rhs.map_pointer) {}
    DequeIterator& operator=(const DequeIterator& rhs)=default;

    reference operator*() const
        {return *ptr;}

    pointer operator->() const
        {return ptr;}
    
    self& operator++() {
        if(++ptr - *map_pointer = block_size) {
            ++map_pointer;
            ptr = *map_pointer;
        }
        return *this;
    }
        
    self& operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }

    self& operator--() {
        if(ptr == *map_pointer) {
            --map_pointer;
            ptr = *map_pointer + block_size;
        }
        --ptr;
        return *this;
    }

    self& operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }

    self& operator+=(difference_type n) {
        if(n != 0){
            n += ptr-*map_pointer;
            if(n>0){
                map_pointer += n/block_size;
                ptr = *map_pointer+n%block_size
            }else{
                difference_type z = block_size-1-n;
                map_pointer -= z/block_size;
                ptr = *map_pointer+(block_size-1-z%block_size);
            }
            return *this;
        }
    }

    self& operator-=(difference_type n) {
        return *this += -n;
    }

    self operator+(difference_type n) const {
        self temp(*this);
        return temp += n;
    }

    self operator-(difference_type n) const {
        self temp(*this);
        return temp -= n;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const self& rhs) const {
        return ptr == rhs.ptr;
    }

    bool operator!=(const self& rhs) const {
        !(*this == rhs);
    }

    bool operator<(const self& rhs) const {
        return map_pointer<rhs.map_pointer || (map_pointer==rhs.map_pointer && ptr<rhs.ptr);
    }

    bool operator>(const self& rhs) const {
        return rhs < *this;
    }

    bool operator<=(const self& rhs) const {
        return !(*this > rhs);
    }

    bool operator>=(const self& rhs) const {
        return !(*this < rhs);
    }

private:
    pointer     ptr;
    map_pointer map_ptr;

    static const size_type block_size = Deque<T>::block_size;

friend class Deque<T>;
};

template <typename T>
class Deque {
public:
    using value_type             = T;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = DequeIterator<T>;
    using const_iterator         = const DequeIterator<T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using map_pointer            = pointer*;

    Deque();
    Deque(size_type);
    Deque(size_type, const T&);
    template <typename InputIterator>
    Deque(InputIterator, InputIterator);
    Deque(std::initializer_list<T>);
    Deque(const Deque&);
    Deque(Deque&&);
    Deque& operator=(const Deque<T>&);
    Deque& operator=(Deque<T>&&);
    ~Deque();

    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](int pos);
    const_reference operator[](int pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    bool empty() const;
    size_type size() const;
    
    void clear();
    iterator insert(const_iterator, const T&);
    iterator insert(const_iterator, T&&);
    iterator insert(const_iterator, size_type, const T&);
    template <typename InputIterator>
    iterator insert(const_iterator, InputIterator, InputIterator);
    iterator insert(const_iterator, std::initializer_list<T>);
    iterator erase(const_iterator);
    iterator erase(const_iterator, const_iterator);
    void push_back(const T&);
    void push_back(T&&);
    void pop_back();
    void push_front(const T&);
    void push_front(T&&);
    void pop_front();
    void resize(size_type);
    void resize(size_type, const T&);
    void swap(Deque<T>&); 

private:
    iterator start;
    iterator finish;
    map_pointer map;
    
    const static size_type block_size = 8;

    void reallocate();
};
