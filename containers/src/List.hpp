#pragma once

#include <cstddef>
#include <cassert>
#include <algorithm>

template <typename T> class ListIterator;
template <typename T> class List;

template<typename T>
struct ListNode {
    ListNode<T>* prev;
    ListNode<T>* next;
    T data;
};

template<typename T>
class ListIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = T;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;
    using link_type         = ListNode<T>*;
    using self              = ListIterator<T>;

    ListIterator(): node{} {};
    ListIterator(link_type x): node(x) {}
    ListIterator(const ListIterator& x)=default;
    ListIterator& operator=(const ListIterator& x)=default;
    
    self& operator++() {
        node = node->next;
        return *this;
    } 

    self operator++(int) {
        self tmp = *this;
        node = node->next;
        return tmp;
    }

    self& operator--() {
        node = node->prev;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        node = node->prev;
        return tmp;
    }

    self operator+=(difference_type n) {
        for(difference_type i=0; i<n; ++i)
            ++(*this);
        return *this;
    }

    self operator-=(difference_type n) {
        for(difference_type i=0; i<n; ++i)
            --(*this);
        return *this;
    }

    self operator+(difference_type n) const {
        self tmp = *this;
        return tmp += n;
    }

    self operator-(difference_type n) const {
        self tmp = *this;
        return tmp -= n;
    }

    reference operator*() const {
        return node->data;
    }

    pointer operator->() const {
        return &(operator*());
    }

    bool operator==(const self& x) const {
        return node == x.node;
    }

    bool operator!=(const self& x) const {
        return node != x.node;
    }

private:
    link_type node;

friend class List<T>;
};

template<typename T>
class List {
public:
    using value_type             = T;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = ListIterator<T>;
    using const_iterator         = const ListIterator<T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using link_type              = ListNode<T>*;

    List();
    List(size_type);
    List(size_type, const value_type&);
    template<typename InputIt>
    List(InputIt, InputIt);
    List(const List&);
    List(List<T>&&);
    List(std::initializer_list<T>);
    List& operator=(const List<T>&);
    List& operator=(List<T>&&);
    ~List();

    reference front()
        {assert(!empty() && "List::front called on empty list");
        return node->next->data;}
    
    const_reference front() const
        {assert(!empty() && "List::front called on empty list");
        return node->next->data;}
    
    reference back()
        {assert(!empty() && "List::back called on empty list");
        return node->prev->data;}
    
    const_reference back() const
        {assert(!empty() && "List::back called on empty list");
        return node->prev->data;}
    
    iterator begin()
        {return iterator(node->next);}
    
    const_iterator begin() const
        {return const_iterator(node->next);}

    const_iterator cbegin() const
        {return const_iterator(node->next);}
    
    iterator end()
        {return iterator(node);}
    
    const_iterator end() const
        {return const_iterator(node);}
    
    const_iterator cend() const
        {return const_iterator(node);}

    reverse_iterator rbegin()
        {return reverse_iterator(end());}
    
    const_reverse_iterator rbegin() const
        {return const_reverse_iterator(end());}
    
    const_reverse_iterator crbegin() const
        {return const_reverse_iterator(cend());}
    
    reverse_iterator rend()
        {return reverse_iterator(begin());}
    
    const_reverse_iterator rend() const
        {return const_reverse_iterator(begin());}
    
    const_reverse_iterator crend() const
        {return const_reverse_iterator(cbegin());}
    
    bool empty() const
        {return node->next == node;}

    size_type size() const
        {return std::distance(begin(), end());}
    
    void clear();
    void push_back(const value_type&);
    void push_back(value_type&&);
    void pop_back();
    void push_front(const value_type&);
    void push_front(value_type&&);
    void pop_front();
    iterator insert(const_iterator, const value_type&);
    iterator insert(const_iterator, value_type&&);
    iterator insert(const_iterator, size_type, const value_type&);
    template<typename InputIt>
    iterator insert(const_iterator, InputIt, InputIt);
    iterator insert(const_iterator, std::initializer_list<T>);
    iterator erase(const_iterator);
    iterator erase(const_iterator, const_iterator);
    void resize(size_type);
    void resize(size_type, const value_type&);
    void swap(List<T>&);
    void splice(const_iterator, List<T>&);
    void splice(const_iterator, List<T>&, const_iterator);
    void splice(const_iterator, List<T>&, const_iterator, const_iterator);
    void remove(const value_type&);
    size_type unique();
    void merge(List<T>&);
    template<typename BinaryPred>
    void merge(List<T>&, BinaryPred);
    void reverse();

private:
    link_type node;
};

template <typename T>
List<T>::List()
    : node(new ListNode<T> {nullptr, nullptr, T()}) 
{
    node->prev=node;
    node->next=node;
}

template <typename T>
List<T>::List(size_type size)
    : node(new ListNode<T> {nullptr, nullptr, T()})
{
    node->prev=node;
    node->next=node;
    for(size_type i=0; i<size; ++i)
        push_back(T());
}

template <typename T>
List<T>::List(size_type size, const value_type& value)
    : node(new ListNode<T> {nullptr, nullptr, T()})
{
    node->prev=node;
    node->next=node;
    for(size_type i=0; i<size; ++i)
        push_back(value);
}

template <typename T>
template <typename InputIt>
List<T>::List(InputIt first, InputIt last)
    : node(new ListNode<T> {nullptr, nullptr, T()})
{
    node->prev=node;
    node->next=node;
    for(auto it=first; it!=last; ++it)
        push_back(*it);
}

template <typename T>
List<T>::List(const List<T>& x)
    : node(new ListNode<T> {nullptr, nullptr, T()})
{
    node->prev=node;
    node->next=node;
    for(auto it=x.begin(); it!=x.end(); ++it)
        push_back(*it);
}

template <typename T>
List<T>::List(List<T>&& x)
    : node(new ListNode<T> {nullptr, nullptr, T()})
{
    node->prev=node;
    node->next=node;
    swap(x);
}

template <typename T>
List<T>::List(std::initializer_list<T> il)
    : node(new ListNode<T> {nullptr, nullptr, T()})
{
    node->prev=node;
    node->next=node;
    for(auto value: il)
        push_back(value);
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& rhs)
{
    if(this==&rhs)
        return;

    node=new ListNode<T> {nullptr, nullptr, T()};
    for(auto it=rhs.begin(); it!=rhs.end(); ++it)
        push_back(*it);
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& rhs)
{
    swap(rhs);
    return *this;
}

template <typename T>
List<T>::~List()
{
    clear();
    delete node;
}

template <typename T>
void List<T>::push_back(const value_type& value)
{
    link_type new_node=new ListNode<T> {node->prev, node, value};
    node->prev->next = new_node;
    node->prev = new_node;
}

template <typename T>
void List<T>::push_back(value_type&& value)
{
    link_type new_node=new ListNode<T> {node->prev, node, std::move(value)};
    node->prev->next = new_node;
    node->prev = new_node;
}

template <typename T>
void List<T>::pop_back()
{
    assert(!empty() && "List::pop_back called on empty list");

    link_type old_node=node->prev;
    node->prev=old_node->prev;
    old_node->prev->next=node;
    delete old_node;
}

template <typename T>
void List<T>::push_front(const value_type& value)
{
    link_type new_node=new ListNode<T> {node, node->next, value};
    node->next->prev = new_node;
    node->next = new_node;
}

template <typename T>
void List<T>::push_front(value_type&& value)
{
    link_type new_node=new ListNode<T> {node, node->next, std::move(value)};
    node->next->prev = new_node;
    node->next = new_node;
}

template <typename T>
void List<T>::pop_front()
{
    assert(!empty() && "List::pop_front called on empty list");

    link_type old_node=node->next;
    node->next=old_node->next;
    old_node->next->prev=node;
    delete old_node;
}

template <typename T>
typename List<T>::iterator List<T>::insert(const_iterator pos, const value_type& value)
{
    link_type new_node=new ListNode<T> {pos.node->prev, pos.node, value};
    pos.node->prev->next=new_node;
    pos.node->prev=new_node;
    return iterator(new_node);
}

template <typename T>
typename List<T>::iterator List<T>::insert(const_iterator pos, value_type&& value)
{
    link_type new_node=new ListNode<T> {pos.node->prev, pos.node, std::move(value)};
    pos.node->prev->next=new_node;
    pos.node->prev=new_node;
    return iterator(new_node);
}

template <typename T>
typename List<T>::iterator List<T>::insert(const_iterator pos, size_type count, const value_type& value)
{
    for(size_type i=0; i<count; ++i)
        pos=insert(pos, value);
    return iterator(pos.node);
}

template <typename T>
template <typename InputIt>
typename List<T>::iterator List<T>::insert(const_iterator pos, InputIt first, InputIt last)
{
    for(auto it=first; it!=last; ++it)
        pos=insert(pos, *it);
    return iterator(pos.node);
}

template <typename T>
typename List<T>::iterator List<T>::insert(const_iterator pos, std::initializer_list<T> il)
{
    for(auto value: il)
        pos=insert(pos, value);
    return iterator(pos.node);
}

template <typename T>
typename List<T>::iterator List<T>::erase(const_iterator pos)
{
    assert(pos!=end() && "List::erase called on end() iterator");

    link_type current=pos.node;
    link_type next=current->next;
    current->prev->next=next;
    next->prev=current->prev;
    delete current;
    return iterator(next);
}

template <typename T>
typename List<T>::iterator List<T>::erase(const_iterator first, const_iterator last)
{
    link_type first_node=first.node;
    link_type last_node=last.node->prev;

    first_node->prev->next=last.node;
    last.node->prev=first_node->prev;

    link_type next=last.node;
    while(first_node!=next){
        link_type temp=first_node;
        first_node=first_node->next;
        delete temp;
    }
    return iterator(next);
}

template <typename T>
void List<T>::clear()
{
    link_type current=node->next;
    while(current!=node){
        link_type temp=current;
        current=current->next;
        delete temp;
    }
    node->next=node;
    node->prev=node;
}

template <typename T>
void List<T>::resize(size_type count)
{
    if(count<size()){
        auto it=begin();
        std::advance(it, count);
        erase(it, end());
    }else
        insert(end(), count-size(), T());
}

template <typename T>
void List<T>::resize(size_type count, const value_type& value)
{
    if(count<size()){
        auto it=begin();
        std::advance(it, count);
        erase(it, end());
    }else
        insert(end(), count-size(), value);
}

template <typename T>
void List<T>::swap(List<T>& x)
{
    std::swap(node, x.node);
}

template <typename T>
void List<T>::splice(const_iterator pos, List<T>& other)
{
    splice(pos, other, other.begin(), other.end());
}

template <typename T>
void List<T>::splice(const_iterator pos, List<T>& other, const_iterator it)
{
    auto next=it;
    ++next;
    splice(pos, other, it, next);
}

template <typename T>
void List<T>::splice(const_iterator pos, List<T>& other, const_iterator first, const_iterator last)
{
    if(first==last)
        return;

    link_type first_node=first.node;
    link_type last_node=last.node->prev;

    first_node->prev->next=last.node;
    last.node->prev=first_node->prev;

    first_node->prev=pos.node->prev;
    last_node->next=pos.node;

    pos.node->prev->next=first_node;
    pos.node->prev=last_node;
}

template <typename T>
void List<T>::remove(const value_type& value)
{
    for(auto it=begin(); it!=end(); ++it)
        if(*it==value)
            erase(it);
}

template <typename T>
auto List<T>::unique() -> size_type
{
    size_type count=0;
    auto it=begin();
    while(it!=end()){
        auto next=it;
        ++next;
        if(*it==*next){
            erase(next);
            ++count;
        }else
            ++it;
    }
    return count;
}

template <typename T>
void List<T>::merge(List<T>& other)
{
    merge(other, std::less<>());
}

template <typename T>
template <typename BinaryPred>
void List<T>::merge(List<T>& other, BinaryPred comp)
{    
    if(this==&other)
        return;

    auto it1=begin();
    auto it2=other.begin();
    while(it1!=end() && it2!=other.end()){
        if(comp(*it2, *it1)){
            auto next=it2;
            ++next;
            splice(it1, other, it2);
            it2=next;
        }else
            ++it1;
    }
    if(it2!=other.end())
        splice(end(), other, it2, other.end());

}

template <typename T>
void List<T>::reverse()
{
    if(size()<2)
        return;

    auto it=begin();
    while(it!=end()){
        auto next=it+1;
        std::swap(it.node->prev, it.node->next);
        it=next;
    }
    std::swap(node->prev, node->next);
}
