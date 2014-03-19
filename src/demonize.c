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

static char **get_process(char **cmd)
{
    int len = 0;
    while (cmd[len])
        len++;
    char **process = malloc((len + 3) * sizeof (char *));
    process[0] = "/bin/sh";
    process[1] = "-c";
    process[2] = malloc(sizeof (char));
    for (int i = 0; i < len; i++)
        process[2] = my_strcat(process[2], cmd[i]);
    process[3] = NULL;
    return process;
}

static void display_help()
{
    printf("Usage: dem [option] [command] ...\n");
    printf("       --cmd, -c: if there is severals commands to demonize, use this option before each of them.\n");
    printf("       --kill, -k: kill the process corresponding to the next index.\n");
    printf("       --jobs, -j: display jobs.\n");
    printf("       --help, -h: displays this help.\n");
}

static char **str_to_wordtab(char *str)
{
    char **ret = malloc(128 * sizeof (char *));
    int j = 0;
    int len = strlen(str);
    while (str && str[0])
    {
        char *newstr = malloc(len * sizeof (char));
        int i = 0;
        while (str[i] == ' ')
            i++;
        int k = 0;
        while (str[i] != ' ')
        {
            newstr[k] = str[i];
            i++;
            k++;
        }
        newstr[k] = '\0';
        str += i;
        ret[j] = newstr;
        j++;
    }
    ret[j] = NULL;
    return ret;
}

static void execute(char **process, char **envp)
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
    {
        char *p = malloc(128 * sizeof (char));
        for (int i = 2; process[i]; i++)
            p = strcat(p, process[i]);
        set_pid(p, f);
        free(p);
    }
}

static void restart(int process, char **envp)
{
    s_pid *pid = get_s_pid(process);
    if (pid)
    {
        kill(pid->pid, SIGKILL);
        delete_pid(process);
        execute(str_to_wordtab(pid->process), envp);
        exit(0);
    }
    else
        fprintf(stderr, "dem: %d: process not found!\n", process);
    exit(1);
}

static s_list *get_args(int argc, char **argv, char **envp)
{
    s_list *list = NULL;
    if (argc <= 1)
    {
        fprintf(stderr, "dem: use --help or -h option\n");
        exit(1);
    }
    else
    {
        for (int i = 1; i < argc; i++)
            if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
            {
                display_help();
                exit(0);
            }
            else if(!strcmp(argv[i], "-j") || !strcmp(argv[i], "--jobs"))
            {
                display_pidlist();
                exit(0);
            }
            else if (!strcmp(argv[i], "--clean"))
                clean();
            else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--kill"))
            {
                if (!(argv + i + 1) || !argv[i + 1])
                {
                    fprintf(stderr, "dem: use --help or -h option\n");
                    exit(1);
                }
                else
                {
                    int pid;
                    if ((pid = get_pid(atoi(argv[i + 1]))) != -1)
                        kill(pid, SIGKILL);
                    else
                        fprintf(stderr, "dem: %d: process doesn't exist!\n", atoi(argv[i + 1]));
                }
                exit(0);
            }
            else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--restart"))
            {
                if (!(argv + i + 1) || !argv[i + 1])
                {
                    fprintf(stderr, "dem: use --help or -h option\n");
                    exit(1);
                }
                else
                    restart(atoi(argv[i + 1]), envp);
            }
            else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cmd"))
                list = add_to_list(i + 1, list);
    }
    return list;
}

void treatment(int argc, char **argv, char **envp)
{
    s_list *positions = get_args(argc, argv, envp);
    char **cmd = malloc(argc * sizeof (char *));
    if (!positions)
    {
        int j = 0;
        for (int i = 1; i < argc; i++)
        {
            cmd[j] = argv[i];
            j++;
        }
        cmd[j] = NULL;
        execute(get_process(cmd), envp);
    }
    else
    {
        s_list *tmp = positions;
        while (tmp)
        {
            int j = 0;
            for (int i = tmp->val; argv[i] && strcmp(argv[i], "-c") && strcmp(argv[i], "--cmd"); i++)
            {
                cmd[j] = argv[i];
                j++;
            }
            cmd[j] = NULL;
            tmp = tmp->next;
            execute(get_process(cmd), envp);
        }
    }
    destroy_list(positions);
}
