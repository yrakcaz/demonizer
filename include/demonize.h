#ifndef DEMONIZE_H
# define DEMONIZE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

void demonize(char **argv, char **envp);

#endif /* !DEMONIZE_H */
