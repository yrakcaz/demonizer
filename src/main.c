#include "../include/demonize.h"

static char *my_strcat(char *str1, char *str2)
{
    char *ret = malloc(1 + strlen(str1) + strlen(str2) * sizeof (char));
    int i = 0;
    while (str1[i])
    {
        ret[i] = str1[i];
        i++;
    }
    ret[i] = ' ';
    i++;
    for (int j = 0; str2[j]; j++)
    {
        ret[i] = str2[j];
        i++;
    }
    ret[i] = '\0';
    return ret;
}

int main(int argc, char **argv, char **envp)
{
    char **process = malloc((argc + 2) * sizeof (char *));
    process[0] = "/bin/sh";
    process[1] = "-c";
    process[2] = malloc(sizeof (char));
    for (int i = 1; i < argc; i++)
        process[2] = my_strcat(process[2], argv[i]);
    process[3] = NULL;
    demonize(process, envp);
    return 0;
}
