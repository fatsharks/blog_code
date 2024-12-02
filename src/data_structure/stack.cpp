#ifndef __DATA_STRUCTURE_STACK_H__
#define __DATA_STRUCTURE_STACK_H__

template<class T> class stack_list
{
private:
    /* data */
    typedef struct list_node_t
    {
        /* data */
        T data;
        list_node_t *next_node;
    } Node_t;
    
public:
    stack_list(/* args */);
    ~stack_list();
};

int stack_test()
{
    return 0;
}

#endif