#pragma once

#include "TriPair.h"
#include <iostream>
#include <iomanip>
using std::cout, std::endl, std::ostream;

template <typename T> class TriPair;
template <typename T> class TriSeqMatrix;
template <typename T> ostream& operator<<(ostream&, const TriSeqMatrix<T>&);
template <typename T> TriSeqMatrix<T> operator+(const TriSeqMatrix<T>&, const TriSeqMatrix<T>&);

template <typename T>
class TriSeqMatrix{
private:
    int capacity;
    int rows, cols, num;
    TriPair<T>* matrix;
public:
    TriSeqMatrix()
        : rows(0), cols(0), num(0), capacity(0), matrix(nullptr) {}
    TriSeqMatrix(int r, int c, int n)
        : rows(r), cols(c), num(0), capacity(n), matrix(new TriPair<T>[n]) {}
    ~TriSeqMatrix()
        {delete[] matrix;}
    TriSeqMatrix(const TriSeqMatrix& rhs)
        : rows(rhs.rows), cols(rhs.cols), num(rhs.num), capacity(rhs.capacity), matrix(new TriPair<T>[rhs.capacity])
        {for(int i=0; i<num; i++) matrix[i]=rhs.matrix[i];}
    TriSeqMatrix operator=(const TriSeqMatrix& rhs);

    // elements access
    int getRows()
        {return rows;}
    int getCols()
        {return cols;}
    int getNum()
        {return num;}
    T getElem(int r, int c);
    void setElem(int r, int c, const T& v);

    // modifiers
    TriSeqMatrix simpleTranspose();
    TriSeqMatrix fastTranspose();

friend ostream& operator<< <>(ostream&, const TriSeqMatrix<T>&);
friend TriSeqMatrix operator+ <>(const TriSeqMatrix<T>&, const TriSeqMatrix<T>&);
};

template <typename T>
auto TriSeqMatrix<T>::operator=(const TriSeqMatrix& rhs) -> TriSeqMatrix
{
    if(this!=&rhs){
        capacity=rhs.capacity;
        rows=rhs.rows;
        cols=rhs.cols;
        num=rhs.num;
        delete[] matrix;
        matrix=new TriPair<T>[rhs.capacity];
        for(int i=0; i<rhs.num; i++)
            matrix[i]=rhs.matrix[i];
    }
    return *this;
}

template <typename T>
T TriSeqMatrix<T>::getElem(int r, int c)
{
    if(r>=rows||c>=cols||r<0||c<0)
        throw("range-fault");
    for(int i=0; i<num; i++)
        if(*(matrix+i).row==r&&*(matrix+i).col==c)
            return *(matrix+i).value;
    return T();
}

template <typename T>
void TriSeqMatrix<T>::setElem(int r, int c, const T& v)
{   
    if(r>=rows||c>=cols||r<0||c<0)
        throw("range-fault");
    for(int i=0; i<num; i++)
        if(matrix[i].row==r&&matrix[i].col==c){
            matrix[i].value=v;
            return;
        }
    // expand capacity if needed
    if(num==capacity){
        int new_capacity=capacity ? 2*capacity : 1;
        TriPair<T>* temp=new TriPair<T>[new_capacity];
        for(int i=0; i<num; i++)
            temp[i]=matrix[i];
        delete[] matrix;
        matrix=temp;
        temp=nullptr;
        capacity=new_capacity;
    }
    // insert sort
    if(!num){
        matrix[0].row=r;
        matrix[0].col=c;
        matrix[0].value=v;
        num++;
    }else{
        int i;
        for(i=num-1; i>=0; i--)
            if(r<matrix[i].row || (r==matrix[i].row && c<matrix[i].col))
                matrix[i+1]=matrix[i];
            else
                break;
        matrix[i+1].row=r;
        matrix[i+1].col=c;
        matrix[i+1].value=v;
        num++;
    }
}

template <typename T>
auto TriSeqMatrix<T>::simpleTranspose() -> TriSeqMatrix
{
    TriSeqMatrix<T> temp(cols, rows, capacity);
    temp.num=num;
    if(num>0){
        int i=0;
        for(int c=0; c<cols; c++)
            for(int j=0; j<num; j++)
                if(matrix[j].col==c){
                    temp.matrix[i].row=matrix[j].col;
                    temp.matrix[i].col=matrix[j].row;
                    temp.matrix[i].value=matrix[j].value;
                    i++;
                }
    }
    return temp;
}

template <typename T>
auto TriSeqMatrix<T>::fastTranspose() -> TriSeqMatrix
{
    TriSeqMatrix<T> temp(cols, rows, capacity);
    temp.num=num;
    int* c_num=new int[cols+1]();
    int* c_pos=new int[cols+1];
    if(num>0){
        for(int i=0; i<num; i++)
            ++c_num[matrix[i].col];
        c_pos[0]=0;
        for(int c=1; c<cols; c++)
            c_pos[c]=c_pos[c-1]+c_num[c-1];
        for(int i=0; i<num; i++){
            int j=c_pos[matrix[i].col];
            temp.matrix[j].row=matrix[i].col;
            temp.matrix[j].col=matrix[i].row;
            temp.matrix[j].value=matrix[i].value;
            ++c_pos[matrix[i].col];
        }
        delete[] c_num;
        delete[] c_pos;
    }
    return temp;
}

template <typename T>
ostream& operator<<(ostream& out, const TriSeqMatrix<T>& rhs)
{
    // for(int i=0; i<rhs.num; i++)
    //     out<<rhs.matrix[i]<<endl;
    T** mat=new T*[rhs.rows]();
    for(int i=0; i<rhs.rows; i++)
        *(mat+i)=new T[rhs.cols]();
    for(int i=0; i<rhs.num; i++)
        mat[rhs.matrix[i].row][rhs.matrix[i].col]=rhs.matrix[i].value;
    for(int i=0; i<rhs.rows; i++){
        for(int j=0; j<rhs.cols; j++)
            cout<<std::setw(3)<<mat[i][j]<<" ";
        cout<<endl;
    }
    return out;
}


template <typename T>
auto operator+(const TriSeqMatrix<T>& lhs,const TriSeqMatrix<T>& rhs) -> TriSeqMatrix<T>
{
    if(lhs.rows!=rhs.rows || lhs.cols!=rhs.cols)
        throw "math-error";
    TriSeqMatrix<T> temp(lhs.rows, rhs.rows, lhs.capacity+rhs.capacity);
    int i=0, j=0, k=0;
    while(i<lhs.num && j<rhs.num){
        if(lhs.matrix[i].row==rhs.matrix[j].row && lhs.matrix[i].col==rhs.matrix[j].col){
            temp.matrix[k].row=lhs.matrix[i].row;
            temp.matrix[k].col=lhs.matrix[i].col;
            temp.matrix[k].value=lhs.matrix[i].value+rhs.matrix[j].value;
            k++;
            i++;
            j++;
        }else if(lhs.matrix[i].row<rhs.matrix[j].row || (lhs.matrix[i].row==rhs.matrix[j].row && lhs.matrix[i].col<rhs.matrix[j].col)){
            temp.matrix[k].row=lhs.matrix[i].row;
            temp.matrix[k].col=lhs.matrix[i].col;
            temp.matrix[k].value=lhs.matrix[i].value;
            k++;
            i++;
        }else{
            temp.matrix[k].row=rhs.matrix[j].row;
            temp.matrix[k].col=rhs.matrix[j].col;
            temp.matrix[k].value=rhs.matrix[j].value;
            k++;
            j++;
        }
    }
    while(i<lhs.num){
        temp.matrix[k].row=lhs.matrix[i].row;
        temp.matrix[k].col=lhs.matrix[i].col;
        temp.matrix[k].value=lhs.matrix[i].value;
        k++;
        i++;
    }
    while(j<rhs.num){
        temp.matrix[k].row=rhs.matrix[j].row;
        temp.matrix[k].col=rhs.matrix[j].col;
        temp.matrix[k].value=rhs.matrix[j].value;
        k++;
        j++;
    }
    temp.num=k;
    return temp;
}

