#include "../include/tools.h"

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

void my_kill(int pid)
{
    char *kill = malloc(16 * sizeof (char));
    sprintf(kill, "kill -9 %d", pid);
    system(kill);
}
