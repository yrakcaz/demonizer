#ifndef DEMONIZE_H
# define DEMONIZE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <signal.h>
# include "manage.h"
# include "tools.h"

/**
** @fn int kill(pid_t pid, int sig);
** @brief Just glibc kill prototype to make it compile.
*/
int kill(pid_t pid, int sig);

/**
** @fn void treatment(int argc, char **argv);
** @brief Principal function which calls get_args static routine and treat options properly.
** @param argc Number of arguments passed to the demonizer.
** @param argv Arguments wordtab passed to the demonizer.
*/
void treatment(int argc, char **argv);


#endif /* !DEMONIZE_H */
