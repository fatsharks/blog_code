#ifndef __DATA_STRUCTURE_LIST_H__
#define __DATA_STRUCTURE_LIST_H__

#include <iostream>

template<class T> class single_list
{
private:
    typedef struct list_node_t
    {
        T data;
        list_node_t *next_node;
    } Node_t;

    Node_t *node;

    
public:
    single_list(/* args */);
    ~single_list();
};

template<class T> class double_list
{
private:
    typedef struct list_node_t
    {
        /* data */
        T data;
        list_node_t *pre_node;
        list_node_t *next_node;
    } Node_t;
    Node_t *node;
public:
    double_list(/* args */);
    ~double_list();
};




int list_test()
{
    return 0;
}


#endif