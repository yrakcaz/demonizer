#include "../include/demonize.h"


void demonize(char **process, char **envp)
{
    pid_t f = fork();
    if (!f)
    {
        int ret = execve(process[0], process, envp);
        if (ret < 0)
        {
            fprintf(stderr, "dem: %s: an error has occurred\n", process[0]);
            exit(1);
        }
    }
    else
        exit(0);
}
