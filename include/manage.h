#ifndef MANAGE_H
# define MANAGE_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h>
# include <fcntl.h>
# include <dirent.h>
# include <string.h>

# define DEAMONS_SIZE 4096

pid_t deamons[DEAMONS_SIZE];

void display_deamons();

#endif /* !MANAGE_H */
