#pragma once

#include "Vector.h"
#include <iostream>
#include <string.h>
using std::ostream, std::cout, std::cin, std::endl;

class String: public Vector<char>{
public:
    using iterator=Vector<char>::iterator;
    using const_iterator=Vector<char>::const_iterator;
    String()=default;
    String(int n): Vector(n) {}
    String(int n, const char value): Vector(n, value) {}
    String(char* first, char* last): Vector(first, last) {}
    String(const Vector<char>& rhs): Vector(rhs) {}
    String(const char*);
    String(const String&);
    String& operator=(const String& rhs);
    virtual ~String()=default;
    String& operator+=(const String&);

friend String operator+(const String&, const String&);
friend ostream& operator<<(ostream&, const String&);
};

String::String(const char* src)
: Vector(strlen(src))
{
    for(int i=0; i<strlen(src); i++)
        (*this)[i]=src[i];
}

String::String(const String& rhs)
: Vector(rhs.size())
{
    int cnt=0;
    for(Vector<char>::iterator it=begin(); it!=end(); ++it)
        *it=rhs[cnt++];
}

String& String::operator+=(const String& rhs)
{
    for(int i=0; i<rhs.size(); i++)
        this->push_back(rhs[i]); 
    return *this;
}

String& String::operator=(const String& rhs)
{
    this->clear();
    for(int i=0; i<rhs.size(); i++)
        this->push_back(rhs[i]); 
    return *this;
}

String operator+(const String& lhs, const String& rhs)
{
    int cnt=0;
    String temp(lhs.size()+rhs.size());
    String::iterator it=temp.begin();
    for(int i=0; i<lhs.size(); i++)
        *it++=lhs[i];
    for(int i=0; i<rhs.size(); i++)
        *it++=rhs[i];
    return temp;
}

ostream& operator<<(ostream& out, const String& rhs)
{
    for(String::const_iterator it=rhs.begin(); it!=rhs.end(); ++it)
        out<<*it;
    return out;
}

