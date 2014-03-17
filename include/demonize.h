#ifndef DEMONIZE_H
# define DEMONIZE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <signal.h>
# include "pid_file.h"
# include "tools.h"

void treatment(int argc, char **argv, char **envp);

#endif /* !DEMONIZE_H */
