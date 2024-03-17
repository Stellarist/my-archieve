#pragma once

#include "TriSeqMatrix.h"
#include <iostream>
using std::cout, std::endl, std::ostream;
template <typename T> class TriSeqMatrix;
template <typename T> ostream& operator<<(ostream&, const TriPair<T>&);
template <typename T> ostream& operator<<(ostream&, const TriSeqMatrix<T>&);
template <typename T> TriSeqMatrix<T> operator+(const TriSeqMatrix<T>&, const TriSeqMatrix<T>&);

template <typename T>
class TriPair{
private:
    int row, col;
    T value;
public:
    TriPair()=default;
    TriPair(int m, int n, const T& new_value)
        : row(m), col(n), value(new_value) {}
    ~TriPair()=default;
    TriPair& operator=(const TriPair&)=default;
    int compareLocation(const TriPair& rhs);
friend class TriSeqMatrix<T>;
// friend ostream& operator<< <>(ostream&, const TriPair<T>&);
friend ostream& operator<< <>(ostream&, const TriSeqMatrix<T>&);
friend TriSeqMatrix<T> operator+ <>(const TriSeqMatrix<T>&, const TriSeqMatrix<T>&);
};

