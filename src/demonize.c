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

static s_list *get_args(int argc, char **argv)
{
    s_list *list = NULL;
    if (argc <= 1)
    {
        fprintf(stderr, "dem: use --help or -h option");
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
                    fprintf(stderr, "dem: use --help or -h option");
                    exit(1);
                }
                else
                    kill(get_pid(atoi(argv[i + 1])), SIGKILL);
                exit(0);
            }
            else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cmd"))
                list = add_to_list(i + 1, list);
    }
    return list;
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
        set_pid(process[0], f);
}

void treatment(int argc, char **argv, char **envp)
{
    s_list *positions = get_args(argc, argv);
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
}
