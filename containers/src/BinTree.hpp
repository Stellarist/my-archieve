#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>

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

template <typename T>
struct TNode{
    T value;
    TNode* child;
    TNode* brother;

    TNode()
        : child(nullptr), brother(nullptr) {}
    TNode(T value)
        : value(value), child(nullptr), brother(nullptr) {}
    TNode(TNode& child, TNode& brother)
        : child(child), brother(brother) {}
    TNode(const TNode&)=default;
    TNode& operator=(const TNode&)=default;
};

template <typename  T>
class BinTree{
private:
    BNode<T>* root;
public:
    BinTree()
        : root(nullptr) {}
    BinTree(const T& r)
        : root(new BNode<T>(r)) {}
    BinTree(const BinTree&)=delete;
    ~BinTree()
        {delete root;}
    BinTree& operator=(const BinTree&)=delete;
    void preorderTraversal(BNode<T>*) const;
    void inorderTraversal(BNode<T>*) const;
    void postorderTraversal(BNode<T>*) const;
    void levelorderTraversal(BNode<T>*) const;
    void destroy(BNode<T>*&);
    bool isEmpty()
        {return root ? 1 : 0;}
    T& getElement(const BNode<T>* node) const
        {return node->value;}
    void setElement(BNode<T>* node, const T& new_value)
        {if(node) node->value=new_value;}
    BNode<T>* getRoot() const
        {return root;}
    BNode<T>* getParent(BNode<T>*, const BNode<T>*) const;
    BNode<T>* getLeftChild(const BNode<T>* node) const
        {return node->left;}
    BNode<T>* getRightChild(const BNode<T>* node) const
        {return node->right;}
    BNode<T>* getLeftSibling(const BNode<T>* node) const
        {return getParent(root, node)->left;}
    BNode<T>* getRightSibling(const BNode<T>* node) const
        {return getParent(root, node)->right;}
    void insertLeftChild(BNode<T>* node, const T& value);
    void insertRightChild(BNode<T>* node, const T& value);
    void deleteLeftChild(BNode<T>* node);
    void deleteRightChild(BNode<T>* node);
    bool isLastLeaf(BNode<T>* node) const;
    int count(BNode<T>*) const;
    int depth(BNode<T>*) const;
    int breadth(BNode<T>*) const;
    int leafcount(BNode<T>*) const;
    void revolute(BNode<T>*);
    void nrInorderTraversal() const;
friend void print<>(const BinTree&, BNode<T>*);
friend std::ostream& operator<< <>(std::ostream&, const BinTree&);
};

template <typename  T>
inline auto BinTree<T>::preorderTraversal(BNode<T>* node) const -> void
{
    if(node){
        std::cout<<node->value<<" ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }
}

template <typename  T>
inline auto BinTree<T>::inorderTraversal(BNode<T>* node) const -> void
{
    if(node){
        inorderTraversal(node->left);
        std::cout<<node->value<<" ";
        inorderTraversal(node->right);
    }
}

template <typename  T>
inline auto BinTree<T>::postorderTraversal(BNode<T>* node) const -> void
{
    if(node){
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        std::cout<<node->value<<" ";
    }
}

template <typename  T>
inline auto BinTree<T>::destroy(BNode<T>*& node) -> void
{
    if(node){
        destroy(node->left);
        destroy(node->right);
        delete node;
        node=nullptr;
    }
}

template <typename  T>
inline auto BinTree<T>::getParent(BNode<T>* root, const BNode<T>* node) const -> BNode<T>*
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
inline auto BinTree<T>::insertLeftChild(BNode<T>* node, const T& value) -> void
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
inline auto BinTree<T>::insertRightChild(BNode<T>* node, const T& value) -> void
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
inline auto BinTree<T>::deleteLeftChild(BNode<T>* node) -> void
{
    if(node->left){
        destroy(node->left);
        node->left=nullptr;
    }
}

template <typename  T>
inline auto BinTree<T>::deleteRightChild(BNode<T>* node) -> void
{
    if(node->right){
        destroy(node->right);
        node->right=nullptr;
    }
}

template <typename T>
inline auto BinTree<T>::count(BNode<T>* node) const -> int
{
    static int cnt=0;
    if(!node)
        return 0;
    else{
        cnt++;
        count(node->left);
        count(node->right);
        return cnt;
    }
}

template <typename T>
inline auto BinTree<T>::depth(BNode<T>* node) const -> int
{
    int left_depth;
    int right_depth;
    if(!node)
        return 0;
    else{
        left_depth=depth(node->left);
        right_depth=depth(node->right);
        return left_depth>=right_depth ? left_depth+1: right_depth+1;
    }
}

template <typename T>
inline auto BinTree<T>::breadth(BNode<T>* root) const -> int
{
    int max=1;
    int last_width=1;
    int present_width=0;
    if(!root)
        return NULL;
    BNode<T>* present;
    std::queue<BNode<T>*> q;
    q.push(root);
    while(!q.empty()){
        present=q.front();
        q.pop();
        if(present->left){
            q.push(present->left);
            present_width++;
        }
        if(present->right){
            q.push(present->right);
            present_width++;
        }
        last_width--;
        if(!last_width){
            if(present_width>max)
                max=present_width;
            last_width=present_width;
            present_width=0;
        }
    }
    return max;
}

template <typename T>
inline auto BinTree<T>::revolute(BNode<T>* node) -> void
{
    if(!node)
        return;
    else{
        std::swap(node->left, node->right);
        revolute(node->left);
        revolute(node->right);
    }
}

template <typename T>
inline auto BinTree<T>::leafcount(BNode<T>* node) const -> int
{
    static int cnt=0;
    if(!root)
        return 0;
    if(!node)
        return cnt;
    else if(!node->left && !node->right){
        return ++cnt;
    }else{
        leafcount(node->left);
        leafcount(node->right);
        return cnt;
    }
}

template <typename T>
inline auto BinTree<T>::nrInorderTraversal() const -> void
{
    bool flag=1;
    std::stack<BNode<T>*> s;
    if(!root)
        return;
    BNode<T>* present=root;
    s.push(nullptr);
    while(!s.empty()){
        if(present){
            s.push(present);
            present=present->left;
        }else{
            present=s.top();
            if(present){
                std::cout<<present->value<<" ";
                present=present->right;
            }
            s.pop();
        }
    }
}

template  <typename  T>
inline auto BinTree<T>::isLastLeaf(BNode<T>* node) const -> bool
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
auto print(const BinTree<T>& tree, BNode<T>* root) -> void
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
        std::cout<<tp<<root->value<<std::endl;
        cnt++;
        print(tree, root->left);
        print(tree, root->right);
        cnt--;
    }
}

template <typename T>
auto operator<<(std::ostream& out, const BinTree<T>& rhs) -> std::ostream&
{
    print(rhs, rhs.root);
    return out;
}

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


template <typename  T>
class Tree{
private:
    TNode<T>* root;
public:
    Tree()
        : root(nullptr) {}
    Tree(const T& r)
        : root(new TNode<T>(r)) {}
    Tree(const Tree&)=delete;
    ~Tree()
        {destroy(root); delete root;}
    Tree& operator=(const Tree&)=delete;
    bool empty()
        {return root ? 1 : 0 ;}
    T& getElement(const TNode<T>* node) const
        {return node->value;}
    void setElement(TNode<T>* node, const T& new_value)
        {if(node) node->value=new_value;}
    TNode<T>* getRoot() const
        {return root;}
    TNode<T>* getChild(const TNode<T>* node) const
        {return node->child;}
    TNode<T>* getSibling(const TNode<T>* node) const
        {return node->brother;}
    TNode<T>* getParent(TNode<T>*, const TNode<T>*) const;
    void destroy(TNode<T>*&);
    void preorderTraversal(TNode<T>*) const;
    void postorderTraversal(TNode<T>*) const;
    void levelorderTraversal(TNode<T>*) const;
    void insertChild(TNode<T>*, const T&);
    void insertBrother(TNode<T>*, const T&);
    void remove(TNode<T>*);
    int depth(TNode<T>*) const;
    int degree(TNode<T>*) const;
friend void print<>(const Tree&, TNode<T>*);
friend std::ostream& operator<< <>(std::ostream&, const Tree&);
};

template <typename  T>
inline auto Tree<T>::destroy(TNode<T>*& node) -> void
{
    if(node){
        destroy(node->child);
        destroy(node->brother);
        delete node;
        node=nullptr;
    }
}

template <typename  T>
inline auto Tree<T>::preorderTraversal(TNode<T>* node) const -> void
{
    if(node){
        std::cout<<node->value<<" ";
        preorderTraversal(node->child);
        preorderTraversal(node->brother);
    }
}

template <typename  T>
inline auto Tree<T>::postorderTraversal(TNode<T>* node) const -> void
{
    if(node){
        postorderTraversal(node->child);
        std::cout<<node->value<<" ";
        postorderTraversal(node->brother);
    }
}

template <typename T>
inline auto Tree<T>::insertChild(TNode<T>* node, const T& value) -> void
{
    if(!node)
        return;
    else if(!node->child){
        node->child=new TNode<T>(value);
        return;
    }else{
        node=node->child;
        while(node->brother)
            node=node->brother;
        node->brother=new TNode<T>(value);
    }
}

template <typename T>
inline auto Tree<T>::insertBrother(TNode<T>* node, const T& value) -> void
{
    if(!node)
        return;
    while(node->brother)
        node=node->brother;
    node->brother=new TNode<T>(value);
}

template <typename T>
inline auto Tree<T>::remove(TNode<T>* node) -> void
{
    destroy(node->child);
}

template <typename T>
auto Tree<T>::depth(TNode<T>* node) const -> int
{
    if(!node)
        return 0;
    else if(!node->child)
        return 1;
    int max_depth=0;
    return std::max(depth(node->child), depth(node->child->brother))+1;
}

template <typename T>
auto Tree<T>::degree(TNode<T>* node) const -> int
{
    if(!node)
        return 0;
    int max_degree=0;
    std::queue<TNode<T>*> q;
    q.push(node);
    while(!q.empty()){
        int cnt {0};
        TNode<T>* p=q.front();
        TNode<T>* c=nullptr;
        q.pop();
        if(!p->child)
            continue;
        else{
            c=p->child;
            while(c){
                q.push(c);
                cnt++;
                c=c->brother;
            } 
            if(cnt>max_degree)
                max_degree=cnt;
        }
    }
    return max_degree;
}

template <typename T>
auto print(const Tree<T>& tree, TNode<T>* root) -> void
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
        if(!root->brother){
            *(t.end()-4)=' ';
        }
        std::string tp=t;
        *(tp.end()-4)='|';
        for(auto it=tp.end()-3; it!=tp.end(); it++)
            *it='-';
        std::cout<<tp<<root->value<<std::endl;
        cnt++;
        print(tree, root->child);
        cnt--;
        print(tree,root->brother);
    }
}

template <typename T>
auto operator<<(std::ostream& out, const Tree<T>& rhs) -> std::ostream&
{
    print(rhs, rhs.root);
    return out;
}
