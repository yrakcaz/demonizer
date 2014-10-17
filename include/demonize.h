#ifndef DEMONIZE_H
# define DEMONIZE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <signal.h>
# include "pid_file.h"
# include "manage.h"
# include "tools.h"

int kill(pid_t pid, int sig);
void treatment(int argc, char **argv);

#endif /* !DEMONIZE_H */
