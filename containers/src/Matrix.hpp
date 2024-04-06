#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

struct Pair{
    int col;
    int element;
    Pair()=default;
    Pair(int n, int new_element)
        : col(n), element(new_element) {}
    Pair operator=(const Pair& rhs)
        {col=rhs.col; element=rhs.element; return *this;}
};

template <typename T>
struct TriPair{
    int row, col;
    T value;
    TriPair()=default;
    TriPair(int m, int n, const T& new_value)
        : row(m), col(n), value(new_value) {}
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
};

template <typename T>
class Matrix{
private:
    int rows, cols;
    std::vector<std::vector<Pair<T>>> row_index;
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
friend std::ostream& operator<<(std::ostream&, const Matrix&);
friend Matrix operator+(const Matrix&, const Matrix&);
};

template <typename T>
auto Matrix<T>::getElem(int r, int c) -> int
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

template <typename T>
auto Matrix<T>::setElem(int r, int c, const int& value) -> void
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

template <typename T>
Matrix<T> Matrix<T>::transpose()
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

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& rhs)
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
        out<<std::endl;
    }
    for(int i=0; i<rhs.rows; i++)
        delete[] *(temp+i);
    delete[] temp;
    return out;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    Matrix<T> temp(lhs.rows, lhs.cols);
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

template <typename T>
class ArraySparseMatrix{
private:
    int capacity;
    int rows, cols, num;
    TriPair<T>* matrix;
public:
    ArraySparseMatrix()
        : rows(0), cols(0), num(0), capacity(0), matrix(nullptr) {}
    ArraySparseMatrix(int r, int c, int n)
        : rows(r), cols(c), num(0), capacity(n), matrix(new TriPair<T>[n]) {}
    ~ArraySparseMatrix()
        {delete[] matrix;}
    ArraySparseMatrix(const ArraySparseMatrix& rhs)
        : rows(rhs.rows), cols(rhs.cols), num(rhs.num), capacity(rhs.capacity), matrix(new TriPair<T>[rhs.capacity])
        {for(int i=0; i<num; i++) matrix[i]=rhs.matrix[i];}
    ArraySparseMatrix operator=(const ArraySparseMatrix& rhs);

    int getRows()
        {return rows;}
    int getCols()
        {return cols;}
    int getNum()
        {return num;}
    T getElem(int r, int c);
    void setElem(int r, int c, const T& v);

    ArraySparseMatrix simpleTranspose();
    ArraySparseMatrix fastTranspose();

friend ostream& operator<< <>(ostream&, const ArraySparseMatrix<T>&);
friend ArraySparseMatrix operator+ <>(const ArraySparseMatrix<T>&, const ArraySparseMatrix<T>&);
};

template <typename T>
auto ArraySparseMatrix<T>::operator=(const ArraySparseMatrix& rhs) -> ArraySparseMatrix
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
T ArraySparseMatrix<T>::getElem(int r, int c)
{
    if(r>=rows||c>=cols||r<0||c<0)
        throw("range-fault");
    for(int i=0; i<num; i++)
        if(*(matrix+i).row==r&&*(matrix+i).col==c)
            return *(matrix+i).value;
    return T();
}

template <typename T>
void ArraySparseMatrix<T>::setElem(int r, int c, const T& v)
{   
    if(r>=rows||c>=cols||r<0||c<0)
        throw("range-fault");
    for(int i=0; i<num; i++)
        if(matrix[i].row==r&&matrix[i].col==c){
            matrix[i].value=v;
            return;
        }

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
auto ArraySparseMatrix<T>::simpleTranspose() -> ArraySparseMatrix
{
    ArraySparseMatrix<T> temp(cols, rows, capacity);
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
auto ArraySparseMatrix<T>::fastTranspose() -> ArraySparseMatrix
{
    ArraySparseMatrix<T> temp(cols, rows, capacity);
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
std::ostream& operator<<(std::ostream& out, const ArraySparseMatrix<T>& rhs)
{
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
auto operator+(const ArraySparseMatrix<T>& lhs,const ArraySparseMatrix<T>& rhs) -> ArraySparseMatrix<T>
{
    if(lhs.rows!=rhs.rows || lhs.cols!=rhs.cols)
        throw "math-error";
    ArraySparseMatrix<T> temp(lhs.rows, rhs.rows, lhs.capacity+rhs.capacity);
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

template <typename T>
class ListSparseMatrix{
private:
    int rows, cols, num;
    CNode<T>** row_pointer, **col_pointer;
public:
    ListSparseMatrix()
        : rows(0), cols(0), num(0), row_pointer(nullptr), col_pointer(0) {}
    ListSparseMatrix(int r, int c)
        : rows(r), cols(c), num(0), row_pointer(new CNode<T>*[rows]()), col_pointer(new CNode<T>*[cols]()) {}
    ListSparseMatrix(int, int, int, CNode<T>*, CNode<T>*);
    ListSparseMatrix(const ListSparseMatrix&);
    ~ListSparseMatrix()
        {delete[] row_pointer; delete[] col_pointer;}
    ListSparseMatrix& operator=(const ListSparseMatrix&);
    constexpr int getRows()
        {return rows;}
    constexpr int getCols()
        {return cols;}
    constexpr int getNum()
        {return num;}
    void setElem(int, int, const T& );
    T getElem(int, int);
    ListSparseMatrix transpose();
friend ListSparseMatrix operator+ <>(const ListSparseMatrix&, const ListSparseMatrix&);
friend std::ostream& operator<< <>(std::ostream&, const ListSparseMatrix&);
};

template <typename T>
inline ListSparseMatrix<T>::ListSparseMatrix(int r, int c, int n, CNode<T>* row_pointer, CNode<T>* col_pointer)
    : rows(r), cols(c), num(n), row_pointer(new CNode<T>*[rows]()), col_pointer(new CNode<T>[cols]())
{
    for(size_t r=0; r<rows; r++)
        this->row_pointer[r]=row_pointer[r];
    for(size_t c=0; c<cols; c++)
        this->col_pointer[c]=col_pointer[c];
}

template <typename T>
inline ListSparseMatrix<T>::ListSparseMatrix(const ListSparseMatrix& rhs)
    : rows(rhs.rows), cols(rhs.cols), num(rhs.num), row_pointer(new CNode<T>* [rhs.rows]), col_pointer(new CNode<T>* [rhs.cols])
{
    for(size_t r=0; r<rows; r++)
        this->row_pointer[r]=rhs.row_pointer[r];
    for(size_t c=0; c<cols; c++)
        this->col_pointer[c]=rhs.col_pointer[c];
}

template <typename T>
inline auto ListSparseMatrix<T>::operator=(const ListSparseMatrix& rhs) -> ListSparseMatrix&
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
auto ListSparseMatrix<T>::setElem(int r, int c, const T& v) -> void
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
inline auto ListSparseMatrix<T>::getElem(int r, int c) -> T
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
auto ListSparseMatrix<T>::transpose() -> ListSparseMatrix<T>
{
    ListSparseMatrix<T> mat_temp(*this);
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
ListSparseMatrix<T> operator+(const ListSparseMatrix<T>& lhs, const ListSparseMatrix<T>& rhs)
{
    if(lhs.rows!=rhs.rows || lhs.cols!=rhs.cols)
        throw "math-error";
    T value_temp;
    ListSparseMatrix<T> mat_temp(lhs.rows, lhs.cols);
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
std::ostream& operator<<(std::ostream& out, const ListSparseMatrix<T>& rhs)
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
