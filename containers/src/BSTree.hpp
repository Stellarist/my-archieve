#pragma once

#include <iostream>
#include <stack>

template <typename T> struct BNode;
template <typename T> struct CrossList;
template <typename T> class BSTree;
template <typename T> void print(const BSTree<T>&, BNode<T>*);
template <typename T> std::vector<int> rowleaf(BNode<T>*);
template <typename T> std::ostream& operator<<(std::ostream&, const BSTree<T>&);

template <typename  T>
struct BNode{
    T value;
    BNode* left;
    BNode* right;

    BNode()
        : left(nullptr), right(nullptr) {}
    BNode(T value)
        : value(value), left(nullptr), right(nullptr) {}
    BNode(BNode& left_child, BNode& right_child)
        : left(left_child), right(right_child) {}
    BNode(const BNode&)=default;
    BNode& operator=(const BNode&)=default;
};

template <typename  T>
class BSTree{
private:
    BNode<T>* root;
public:
    BSTree()
        : root(nullptr) {}
    BSTree(const T& r)
        : root(new BNode<T>(r)) {}
    BSTree(const BSTree&)=delete;
    ~BSTree()
        {delete root;}
    BSTree& operator=(const BSTree&)=delete;
    void destroy(BNode<T>*&);
    bool isEmpty()
        {return root ? 1 : 0;}
    BNode<T>* getRoot() const
        {return root;}
    BNode<T>* getParent(BNode<T>*, const BNode<T>*) const;
    void insertLeftChild(BNode<T>* node, const T& value);
    void insertRightChild(BNode<T>* node, const T& value);
    void deleteLeftChild(BNode<T>* node);
    void deleteRightChild(BNode<T>* node);
    bool isLastLeaf(BNode<T>* node) const;
    bool isSorted() const;
    void insert(const T&);
    void remove(BNode<T>*&);
    void inorderTraversal(BNode<T>*) const;
friend void print<>(const BSTree&, BNode<T>*);
friend std::ostream& operator<< <>(std::ostream&, const BSTree&);
};

template <typename T>
inline auto BSTree<T>:: isSorted() const -> bool
{
    bool flag=1;
    if(!root)
        return 1;
    std::stack<BNode<T>*> s;
    auto present=root;
    T previous;
    s.push(nullptr);
    while(!s.empty()){
        if(present){
            s.push(present);
            present=present->left;
        }else{
            if(s.size()==1)
                return 1;
            present=s.top();
            if(present){
                if(flag){
                    flag=0;
                    previous=present->value;
                }
                if(present->value<previous)
                    return 0;
                previous=present->value;
                present=present->right;
                s.pop();
            }
        }
    }
    return 1;
}

template <typename T>
auto BSTree<T>::insert(const T& value) -> void
{
    if(!root)
        root=new BNode<T> (value);
    bool flag=0;
    BNode<T>* present=root;
    BNode<T>* previous;
    while(present){
        previous=present;
        if(present->value>value)
            present=present->left;
        else if(present->value<value)
            present=present->right;
        else
            return;
    }
    if(previous->value>value)
        previous->left=new BNode<T>(value);
    else
        previous->right=new BNode<T>(value);
}

template <typename T>
auto BSTree<T>::remove(BNode<T>*& des) -> void
{
    BNode<T>* previous, * present;
    if(!des->left && !des->right){
        delete des;
        des=nullptr;
    }else if(!des->left){
        present=des;
        des=des->right;
        delete present;
    }else if(!des->right){
        present=des;
        des=des->left;
        delete present;
    }else{
        previous=des;
        present=des->left;
        while(present->right){
            previous=present;
            present=present->right;
        }
        des->value=present->value;
        if(previous->right==present)
            remove(previous->right);
        else
            remove(previous->left);
    }
}

template <typename  T>
inline auto BSTree<T>::destroy(BNode<T>*& node) -> void
{
    if(node){
        destroy(node->left);
        destroy(node->right);
        delete node;
        node=nullptr;
    }
}

template <typename  T>
inline auto BSTree<T>::getParent(BNode<T>* root, const BNode<T>* node) const -> BNode<T>*
{
    if(!root)
        return NULL;
    else if(root->left==node || root->right==node){
        return root;
    }else{
        BNode<T>* temp;
        temp=getParent(root->left, node);
        if(temp)
            return temp;
        temp=getParent(root->right, node);
        if(temp)
            return temp;
        return NULL;
    }
}

template <typename  T>
inline auto BSTree<T>::insertLeftChild(BNode<T>* node, const T& value) -> void
{
    if(!node)
        return;
    else{
        BNode<T>* child=new BNode<T>(value);
        if(node->left)
            child->left=node->left;
        node->left=child;
        return;
    }
}

template <typename  T>
inline auto BSTree<T>::insertRightChild(BNode<T>* node, const T& value) -> void
{
    if(!node)
        return;
    else{
        BNode<T>* child=new BNode<T>(value);
        if(node->right)
            child->right=node->right;
        node->right=child;
        return;
    }
}

template <typename  T>
inline auto BSTree<T>::deleteLeftChild(BNode<T>* node) -> void
{
    if(node->left){
        destroy(node->left);
        node->left=nullptr;
    }
}

template <typename  T>
inline auto BSTree<T>::deleteRightChild(BNode<T>* node) -> void
{
    if(node->right){
        destroy(node->right);
        node->right=nullptr;
    }
}

template <typename  T>
inline auto BSTree<T>::inorderTraversal(BNode<T>* node) const -> void
{
    if(node){
        inorderTraversal(node->left);
        std::cout<<node->value<<" ";
        inorderTraversal(node->right);
    }
}

template  <typename  T>
inline auto BSTree<T>::isLastLeaf(BNode<T>* node) const -> bool
{
    BNode<T>* parent=getParent(this->root, node);
    if(!node)
        throw "parameter-fault";
    if(node==root)
        return 1;
    else if(!parent->right)
        return 1;
    else if(parent->right==node)
        return 1;
    return 0;
}

template <typename T>
auto print(const BSTree<T>& tree, BNode<T>* root, bool loc) -> void
{
    static int cnt=0;
    static std::string t;
    if(!root)
        return;
    else{
        if(t.size()<4*(cnt+1)){
            t.resize(4*(cnt+1),' ');
            *(t.end()-4)='|';
        }else if(t.size()>4*(cnt+1))
            t.resize(4*(cnt+1));
        if(tree.isLastLeaf(root)){
            *(t.end()-4)=' ';
        }
        std::string tp=t;
        *(tp.end()-4)='|';
        for(auto it=tp.end()-3; it!=tp.end(); it++)
            *it='-';
        std::cout<<tp<<root->value;
        if(loc)
            std::cout<<"r";
        else
            std::cout<<"l";
        std::cout<<std::endl;
        cnt++;
        print(tree, root->left, 0);
        print(tree, root->right, 1);
        cnt--;
    }
}

template <typename T>
auto operator<<(std::ostream& out, const BSTree<T>& rhs) -> std::ostream&
{
    print(rhs, rhs.root, 0);
    return out;
}

