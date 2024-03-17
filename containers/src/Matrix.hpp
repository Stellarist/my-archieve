#pragma once

#include <iostream>
#include <iomanip>
#include "Vector.h"
using std::ostream, std::cout, std::endl;
class Pair;
class Matrix;

class Pair{
private:
    int col;
    int element;
public:
    Pair()=default;
    Pair(int n, int new_element)
        : col(n), element(new_element) {}
    Pair(const Pair&)=default;
    ~Pair()=default;
    Pair operator=(const Pair& rhs)
        {col=rhs.col; element=rhs.element; return *this;}
friend class Matrix;
friend ostream& operator<<(ostream&, const Matrix&);
friend Matrix operator+(const Matrix&, const Matrix&);
};

class Matrix{
private:
    int rows, cols;
    Vector<Vector<Pair>> row_index;
public:
    Matrix()
        : rows(0), cols(0), row_index() {}
    Matrix(int r, int c)
        : rows(r), cols(c), row_index(rows) {}
    Matrix(const Matrix&)=default;
    ~Matrix()=default;
    Matrix& operator=(const Matrix&)=default;
    int getElem(int, int);
    void setElem(int, int, const int&);
    Matrix transpose();
friend ostream& operator<<(ostream&, const Matrix&);
friend Matrix operator+(const Matrix&, const Matrix&);
};

auto Matrix::getElem(int r, int c) -> int
{
    if(r<0 || r>=rows || c<0 || c>=cols)
        throw "range-fault";
    else{
        int temp=0;
        for(auto it=row_index[r].begin(); it!=row_index[r].end(); it++){
            if(it->col==c){
                temp=it->element;
                return temp;
            }
        }
    return temp;
    }
}

auto Matrix::setElem(int r, int c, const int& value) -> void
{
    if(r<0 || r>=rows || c<0 || c>=cols)
        throw "range-fault";
    else if(row_index[r].empty()){
        Pair temp(c, value);
        row_index[r].push_back(temp);
    }else{
        auto it=row_index[r].begin();
        for(; it!=row_index[r].end(); it++)
            if(it->col>=c)
                break;
        if(it->col==c)
            it->element=value;
        else{
            Pair temp(c,value);
            row_index[r].insert(it, temp);
        }
    }
}

Matrix Matrix::transpose()
{
    Matrix temp(cols, rows);
    for(int i=0; i<rows; i++){
        for(int j=0; j<row_index[i].size(); j++){
            Pair temp_pair(i, row_index[i][j].element);
            temp.row_index[row_index[i][j].col].push_back(temp_pair);
        }
    }
    return temp;
}

ostream& operator<<(ostream& out, const Matrix& rhs)
{
    int** temp=new int* [rhs.rows];
    for(int i=0; i<rhs.rows; i++)
        *(temp+i)=new int[rhs.cols]();
    for(int i=0; i<rhs.rows; i++)
        for(auto it=rhs.row_index[i].begin(); it!=rhs.row_index[i].end(); it++)
            temp[i][it->col]=it->element;
    for(int i=0; i<rhs.rows; i++){
        for(int j=0; j<rhs.cols; j++)
            out<<std::setw(3)<<temp[i][j];
        out<<endl;
    }
    for(int i=0; i<rhs.rows; i++)
        delete[] *(temp+i);
    delete[] temp;
    return out;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    Matrix temp(lhs.rows, lhs.cols);
    for (int i = 0; i < lhs.rows; i++){
        int j=0, k=0;
        while (j<lhs.row_index[i].size() && k<rhs.row_index[i].size()){
            if(lhs.row_index[i][j].col<rhs.row_index[i][k].col){
                Pair new_element(lhs.row_index[i][j].col, lhs.row_index[i][j].element);
                temp.row_index[i].push_back(new_element);
                j++;
            }else if(lhs.row_index[i][j].col>rhs.row_index[i][k].col){
                Pair new_element(rhs.row_index[i][k].col, rhs.row_index[i][k].element);
                temp.row_index[i].push_back(new_element);
                k++;
            }else{
                Pair new_element(lhs.row_index[i][j].col, lhs.row_index[i][j].element+rhs.row_index[i][k].element);
                temp.row_index[i].push_back(new_element);
                j++;
                k++;
            }
        }
        while (j<lhs.row_index[i].size()){
            Pair new_element(lhs.row_index[i][j].col, lhs.row_index[i][j].element);
            temp.row_index[i].push_back(new_element);
            j++;
        }
        while (k<rhs.row_index[i].size()){
            Pair new_element(rhs.row_index[i][k].col, rhs.row_index[i][k].element);
            temp.row_index[i].push_back(new_element);
            k++;
        }
    }
    return temp;
}

