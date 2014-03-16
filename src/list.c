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

void destroy_list(s_list *list)
{
    if (!list)
        return;
    if (!(list->next))
    {
        free(list);
        return;
    }
    s_list *tmp = list;
    while (tmp)
    {
        tmp = tmp->next;
        free(list);
        list = tmp;
    }
}
