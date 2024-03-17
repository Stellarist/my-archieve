#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>

template <typename T> struct BNode;
template <typename T> struct CrossList;
template <typename T> class BinTree;
template <typename T> void print(const BinTree<T>&, BNode<T>*);
template <typename T> std::vector<int> rowleaf(BNode<T>*);
template <typename T> std::ostream& operator<<(std::ostream&, const BinTree<T>&);

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
