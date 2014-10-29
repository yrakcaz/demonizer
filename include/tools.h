#ifndef TOOLS_H
# define TOOLS_H

# include <stdlib.h>
# include <stdio.h>

/**
** @struct list
** @brief List of integers.
*/
typedef struct list
{
    int val;
    struct list *next;
} s_list;

/**
** @fn s_list *add_to_list(int val, s_list *list);
** @brief Adds an element to an integer list.
** @param val Integer added to the list.
** @param list List where to add the element.
** @return Returns the list with added element.
*/
s_list *add_to_list(int val, s_list *list);

/**
** @fn void destroy_list(s_list *list);
** @brief Destroys the list of integer.
** @param list List to be destroyed.
*/
void destroy_list(s_list *list);

/**
** @fn void destroy_matrix(char **mat);
** @brief Destroys a bi-dimmensionnal array of characters.
** @param mat Matrix to be free'd.
*/
void destroy_matrix(char **mat);


#endif /* !TOOLS_H */
