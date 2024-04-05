#pragma once

#include <iostream>
#include <queue>

template <typename T> struct TNode;
template <typename T> class Tree;
template <typename T> void print(const Tree<T>&, TNode<T>*);
template <typename T> std::ostream& operator<<(std::ostream&, const Tree<T>&);

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
