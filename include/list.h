#ifndef LIST_H
# define LIST_H

# include <stdlib.h>

typedef struct list
{
    int val;
    struct list *next;
} s_list;

s_list *add_to_list(int val, s_list *list);
void destroy_list(s_list *list);

#endif /* !LIST_H */
