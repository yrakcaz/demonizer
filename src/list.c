#include "../include/list.h"

s_list *add_to_list(int val, s_list *list)
{
    s_list *node =  malloc(sizeof (s_list));
    node->val = val;
    if (!list)
        node->next = NULL;
    else
        node->next = list;
    return node;
}
