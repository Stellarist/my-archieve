#pragma once

#include <iostream>
#include <algorithm>

template <typename T> struct Tripair;
template <typename T> struct CNode;
template <typename T> class CrossList;
template <typename T> CrossList<T> operator+(const CrossList<T>&, const CrossList<T>&);
template <typename T> std::ostream& operator<<(std::ostream&, const CrossList<T>&);

template <typename T>
struct Tripair{
    int row, col;
    T value;
    Tripair()=default;
    Tripair(int r, int c, T v)
        : row(r), col(c), value(v) {}
};

template <typename T>
struct CNode{
    Tripair<T> value;
    CNode<T>* right, * down;
    CNode()
        : right(nullptr), down(nullptr) {}
    CNode(int r, int c, const T& v)
        : value(r, c, v), right(nullptr), down(nullptr) {}
    CNode(const CNode& rhs)=default;
    CNode& operator*()
        {return *this;}
    CNode* operator->()
        {return this;}
friend CrossList<T> operator+ <>(const CrossList<T>&, const CrossList<T>&);
};

template <typename T>
class CrossList{
private:
    int rows, cols, num;
    CNode<T>** row_pointer, **col_pointer;
public:
    CrossList()
        : rows(0), cols(0), num(0), row_pointer(nullptr), col_pointer(0) {}
    CrossList(int r, int c)
        : rows(r), cols(c), num(0), row_pointer(new CNode<T>*[rows]()), col_pointer(new CNode<T>*[cols]()) {}
    CrossList(int, int, int, CNode<T>*, CNode<T>*);
    CrossList(const CrossList&);
    ~CrossList()
        {delete[] row_pointer; delete[] col_pointer;}
    CrossList& operator=(const CrossList&);
    constexpr int getRows()
        {return rows;}
    constexpr int getCols()
        {return cols;}
    constexpr int getNum()
        {return num;}
    void setElem(int, int, const T& );
    T getElem(int, int);
    CrossList transpose();
friend CrossList operator+ <>(const CrossList&, const CrossList&);
friend std::ostream& operator<< <>(std::ostream&, const CrossList&);
};

template <typename T>
inline CrossList<T>::CrossList(int r, int c, int n, CNode<T>* row_pointer, CNode<T>* col_pointer)
    : rows(r), cols(c), num(n), row_pointer(new CNode<T>*[rows]()), col_pointer(new CNode<T>[cols]())
{
    for(size_t r=0; r<rows; r++)
        this->row_pointer[r]=row_pointer[r];
    for(size_t c=0; c<cols; c++)
        this->col_pointer[c]=col_pointer[c];
}

template <typename T>
inline CrossList<T>::CrossList(const CrossList& rhs)
    : rows(rhs.rows), cols(rhs.cols), num(rhs.num), row_pointer(new CNode<T>* [rhs.rows]), col_pointer(new CNode<T>* [rhs.cols])
{
    for(size_t r=0; r<rows; r++)
        this->row_pointer[r]=rhs.row_pointer[r];
    for(size_t c=0; c<cols; c++)
        this->col_pointer[c]=rhs.col_pointer[c];
}

template <typename T>
inline auto CrossList<T>::operator=(const CrossList& rhs) -> CrossList&
{
    if(this!=&rhs){
        delete[] row_pointer;
        delete[] col_pointer;
        rows=rhs.rows;
        cols=rhs.cols;
        num=rhs.num;
        row_pointer=new CNode<T>*[rhs.rows];
        col_pointer=new CNode<T>* [rhs.cols];
        for(size_t r=0; r<rows; r++)
            this->row_pointer[r]=rhs.row_pointer[r];
        for(size_t c=0; c<cols; c++)
            this->col_pointer[c]=rhs.col_pointer[c];
    }
}

template <typename T>
auto CrossList<T>::setElem(int r, int c, const T& v) -> void
{
    if(r>=rows || c>=cols || r<0 || c<0)
        throw "range-fault";
    CNode<T>*& rf=row_pointer[r];
    CNode<T>* rp=rf;
    CNode<T>*& cf=col_pointer[c];
    CNode<T>* cp=cf;
    CNode<T>* q=nullptr;
    CNode<T>* temp=new CNode<T>(r, c, v);
    if(!rp){
        rf=temp;
        num++;
    }else if(rp->value.col>c){
        CNode<T>* t=new CNode<T>(*rf);
        rf=temp;
        rf->right=t;
        num++;
    }else{
        while(rp && rp->value.col<c){
            q=rp;
            rp=rp->right;
        }
        if(rp && rp->value.col==c){
            rp->value.value=v;
            return;
        }else{
            temp->right=q->right;
            q->right=temp;
            num++;
        }
    }
    if(!cp){
        cf=temp;
        num++;
    }else if(cp->value.row>r){
        CNode<T>* t=new CNode<T>(*cf);
        cf=temp;
        cf->down=t;
    }else{
        q=cp;
        while(cp && cp->value.row<r){
            q=cp;
            cp=cp->down;
        }
        if(cp && cp->value.row==r){
            cp->value.value=v;
        }else{
            temp->down=q->down;
            q->down=temp;
        }
    }
}

template <typename T>
inline auto CrossList<T>::getElem(int r, int c) -> T
{
    if(r>=rows || c>=cols || r<0 || c<0)
        throw "range-fault";
    CNode<T>* p=row_pointer[r];
    while(p){
        if(p->value.col==c)
            return p->value.value;
        else if(p->value.col>c)
            return T();
        p=p->right;
    }
}

template <typename T>
auto CrossList<T>::transpose() -> CrossList<T>
{
    CrossList<T> mat_temp(*this);
    CNode<T>* temp;
    CNode<T>* temp2;
    for(int i=0; i<rows; i++){
        temp=mat_temp.row_pointer[i];
        while(temp){
            temp2=temp;
            temp=temp->right;
            std::swap(temp2->right, temp2->down);
            std::swap(temp2->value.row, temp2->value.col);
        }
    }
    std::swap(mat_temp.row_pointer, mat_temp.col_pointer);
    return mat_temp;
}

template <typename T>
CrossList<T> operator+(const CrossList<T>& lhs, const CrossList<T>& rhs)
{
    if(lhs.rows!=rhs.rows || lhs.cols!=rhs.cols)
        throw "math-error";
    T value_temp;
    CrossList<T> mat_temp(lhs.rows, lhs.cols);
    CNode<T>* p, *q;
    for(int i=0; i<lhs.rows; i++){
        p=lhs.row_pointer[i];
        q=rhs.row_pointer[i];
        while(p && q){
            if(p->value.col<q->value.col){
                mat_temp.setElem(p->value.row, p->value.col, p->value.value);
                p=p->right;
            }else if(p->value.col>q->value.col){
                mat_temp.setElem(q->value.row, q->value.col, q->value.value);
                q=q->right;
            }else{
                value_temp=p->value.value+q->value.value;
                if(value_temp)
                    mat_temp.setElem(q->value.row, q->value.col, value_temp);
                p=p->right;
                q=q->right;
            }
        }
        while(p){
            mat_temp.setElem(p->value.row, p->value.col, p->value.value);
            p=p->right;
        }
        while(q){
            mat_temp.setElem(q->value.row, q->value.col, q->value.value);
            q=q->right;
        }
    }
    return mat_temp;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const CrossList<T>& rhs)
{
    T mat[rhs.rows][rhs.cols];
    for(int i=0; i<rhs.rows; i++)
        for(int j=0; j<rhs.cols; j++)
            mat[i][j]=T();
    for(int i=0; i<rhs.rows; i++){
        CNode<T>* p=rhs.row_pointer[i];
        while(p){
            mat[p->value.row][p->value.col]=p->value.value;
            p=p->right;
        }
    } 
    for(int i=0; i<rhs.rows; i++){
        for(int j=0; j<rhs.cols; j++)
            std::cout<<mat[i][j]<<" ";
        std::cout<<std::endl;
    }
    return out;
}

