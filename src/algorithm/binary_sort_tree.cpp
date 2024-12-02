#ifndef __ALG_BST_H__
#define __ALG_BST_H__

#include <iostream>
#include <queue>
#include <stack>

template<class T> class binary_sort_tree
{
private:
    /* data */
    typedef struct tree_node_t
    {
        /* data */
        T value;
        struct tree_node_t *lc;
        struct tree_node_t *rc;
    } Node_t;
    Node_t *root;
public:
    binary_sort_tree(/* args */);
    ~binary_sort_tree();
    void insert(const T&& arg);
    void preorder_travel_recur(Node_t *r_node);
    void inorder_travel_recur(Node_t *r_node);
    void postorder_travel_recur(Node_t *r_node);

    void level_travel(Node_t *r_node);

    void preorder_travel(Node_t *r_node);  // dfs
    void inorder_travel(Node_t *r_node);

    Node_t *find_node(const T&& value);
    void delete_node(const T&& value);
};

template<class T> void binary_sort_tree<T>::preorder_travel_recur(Node_t *r_node)
{
    if(r_node != nullptr)
    {
        std::cout << "cur node data: " << r_node->value << std::endl;
        preorder_travel_recur(r_node->lc);
        preorder_travel_recur(r_node->rc);
    }
}

template<class T> void binary_sort_tree<T>::inorder_travel_recur(Node_t *r_node)
{
    if(r_node != nullptr)
    {
        preorder_travel_recur(r_node->lc);
        std::cout << "cur node data: " << r_node->value << std::endl;
        preorder_travel_recur(r_node->rc);  
    }
}

template<class T> void binary_sort_tree<T>::postorder_travel_recur(Node_t *r_node)
{
    if(r_node != nullptr)
    {
        postorder_travel_recur(r_node->lc);
        postorder_travel_recur(r_node->rc);
        std::cout << "cur node data: " << r_node->value << std::endl;
    }
}

template<class T> void binary_sort_tree<T>::level_travel(Node_t *r_node)
{
    if(r_node)
    {
        std::queue<Node_t*> que;
        que.emplace(r_node);
        while(!que.empty())
        {
            Node_t *node = que.front();
            que.pop();
            std::cout << "cur node data: " << node->value << std::endl;
            if(node->lc)
            {
                que.emplace(node->lc);
            }
            if(node->rc)
            {
                que.emplace(node->rc);
            }
        }
        return;
    }
}

template<class T> void binary_sort_tree<T>::preorder_travel(Node_t *r_node)
{
    if(r_node)
    {
        std::stack<Node_t> st;
        st.emplace(r_node);
        while(!st.empty())
        {
            Node_t *node = st.top();
            st.pop();
            std::cout << "cur node data: " << node->value << std::endl;
            if(node->rc)
            {
                st.emplace(node->rc);
            }
            if(node->lc)
            {
                st.emplace(node->lc);
            }

        }
    }
} 

template<class T> void binary_sort_tree<T>::inorder_travel(Node_t *r_node)
{
    if(r_node)
    {
        std::stack<Node_t *> st;
        st.emplace(r_node);
        while(!st.empty())
        {
            Node_t *node = st.top();
            if(node->lc)
            {
                st.emplace(node->lc);
            }
            else
            {
                std::cout << "cur node data: " << node->value << std::endl;
                st.pop();
                if(node->rc)
                {
                    st.emplace(node->rc);
                }
            }
        }
    }
}

template<class T> typename binary_sort_tree<T>::Node_t* binary_sort_tree<T>::find_node(const T&& value)
{
}

template<class T> void binary_sort_tree<T>::delete_node(const T&& value)
{
}



int binary_sort_tree_test()
{
    return 0;
}

#endif