#include "../include/tools.h"

void destroy_matrix(char **mat)
{
    int i;
    for (i = 0; mat[i]; i++)
        free(mat[i]);
    free(mat);
}

s_list *add_to_list(int val, s_list *list)
{
    s_list *node =  malloc(sizeof (s_list));
    node->val = val;
    node->next = list;
    return node;
}

void destroy_list(s_list *list)
{
    s_list *tmp = list;
    while (tmp)
    {
        tmp = tmp->next;
        free(list);
        list = tmp;
    }
}
