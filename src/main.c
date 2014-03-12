#include "../include/demonize.h"

int main(int argc, char **argv, char **envp)
{
    char **process = malloc(argc * sizeof (char *));
    int j = 0;
    for (int i = 1; i < argc; i++)
    {
        process[j] = argv[i];
        j++;
    }
    process[j] = NULL;
    demonize(process, envp);
    return 0;
}
