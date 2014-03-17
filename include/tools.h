#ifndef TOOLS_H
# define TOOLS_H

# include <stdlib.h>
# include <stdio.h>

typedef struct list
{
    int val;
    struct list *next;
} s_list;

s_list *add_to_list(int val, s_list *list);
void destroy_list(s_list *list);
void my_kill(int pid);

#endif /* !TOOLS_H */
