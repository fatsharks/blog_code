#ifndef __DATA_STRUCTURE_RBT_H__
#define __DATA_STRUCTURE_RBT_H__

#define NULL nullptr

enum RBColor{
    RED   = 0,
    BLACK = 1
};

template<class T> class rb_tree
{
private:
    /* data */
    typedef struct rb_tree_node_t
    {
        /* data */
        T value;
        RBColor color;
        rb_tree_node_t *l_node;
        rb_tree_node_t *r_node;
        rb_tree_node_t *parent;
    } Node_t;
    
    Node_t *root;

public:
    rb_tree(/* args */){};
    ~rb_tree(){};
    void left_rotate(Node_t *node);
    void right_rotate(Node_t *node);
    void insert_node(const T& value);
    void remove_node(const T& value);
    Node_t *find_node(const T& value);
    void rb_tree_refresh();
};

template<class T> typename rb_tree<T>::Node_t* rb_tree<T>::find_node(const T& value)
{
    if(root == NULL)
    {
        return root;
    }
    else
    {
        Node_t *tmp = root;
        while(tmp)
        {
            if(value == tmp->value)
            {
                return tmp;
            }
            else if(value > tmp->value)
            {
                tmp = tmp->r_node;
            }
            else if(value < tmp->l_node)
            {
                tmp = tmp->l_node;
            }
        }
        return NULL;
    }
}

template<class T> void rb_tree<T>::insert_node(const T& value)
{
    Node_t *new_node = new Node_t{value, RED, NULL, NULL, NULL};

    if(root == NULL)
    {
        root = new_node;
        root->color = BLACK;
        return;
    }
    Node_t *tmp = root;
    Node_t *parent_node = NULL;
    while(tmp)
    {
        parent_node = tmp;
        if(tmp->value == value)
        {
            return;
        }
        else if(value > tmp->value)
        {
            tmp = tmp->r_node;
        }
        else if(value < tmp->value)
        {
            tmp = tmp->r_node;
        }
    }

    new_node->parent = parent_node;
    if(value > parent_node->value)
    {
        parent_node->r_node = new_node;
    }
    else
    {
        parent_node->l_node = new_node;
    }

    rb_tree_refresh();

}

template<class T> void rb_tree<T>::left_rotate(Node_t *node)
{
    if(node && node->r_node)
    {
        Node_t *tmp = node->r_node;

        // 处理 右节点的左子节点
        node->r_node = tmp->l_node;
        if(tmp->l_node)
        {
            tmp->l_node->parent = node;
        }

        // 处理 右节点的父节点关系
        tmp->parent = node->parent;
        if(node->parent == nullptr)
        {
            root = tmp;
        }
        if(node->parent->l_node == node)
        {
            node->parent->l_node = tmp;
        }
        else
        {
            node->parent->r_node = tmp;
        }

        // 处理 左节点的父节点关系
        node->parent = tmp;
        tmp->l_node = node;
    }
    else
    {
        return;
    }
}

template<class T> void rb_tree<T>::right_rotate(Node_t *node)
{
    if(node && node->l_node)
    {
        Node_t *tmp = node->l_node;

        node->l_node = tmp->r_node;
        if(tmp->r_node)
        {
            tmp->r_node->parent = node;
        }

        tmp->parent = node->parent;
        if(node->parent == NULL)
        {
            root = tmp;
        }
        if(node->parent->l_node == node)
        {
            node->parent->l_node = tmp;
        }
        else
        {
            node->parent->r_node = tmp;
        }

        node->parent = tmp;
        tmp->r_node = node;
    }
}

template<class T> void rb_tree<T>::remove_node(const T& value)
{
    Node_t *t_node = find_node(value);
    if(t_node == NULL)
    {
        return;
    }
    else
    {
        // 找到真前驱节点（左子树最右节点）或者真后继节点（右子树最左节点）替换，问题转换成删除真前驱节点或者真后继节点
    }
}





int red_black_tree()
{
    return 0;
}

#endif