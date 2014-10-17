#include "../include/demonize.h"

static char **get_process(char **cmd)
{
    int len = 0;
    int i = 0;
    while (cmd[len])
        len++;
    char **process = malloc((len + 1) * sizeof (char *));
    for (i = 0; i < len; i++)
        process[i] = cmd[i];
    process[i] = NULL;
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

static void execute(char **process)
{
    pid_t f = fork();
    if (!f)
    {
        int ret = execvp(process[0], process);
        if (ret < 0)
        {
            fprintf(stderr, "dem: %s: an error has occurred\n", process[0]);
            exit(1);
        }
        exit(0);
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

static void restart(int process)
{
    s_pid *pid = get_s_pid(process);
    if (pid)
    {
        kill(pid->pid, SIGKILL);
        delete_pid(process);
        execute(str_to_wordtab(pid->process));
        exit(0);
    }
    else
        fprintf(stderr, "dem: %d: process not found!\n", process);
    exit(1);
}

static s_list *get_args(int argc, char **argv)
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
                display_deamons();
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
                    restart(atoi(argv[i + 1]));
            }
            else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cmd"))
                list = add_to_list(i + 1, list);
    }
    return list;
}

void treatment(int argc, char **argv)
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
        execute(get_process(cmd));
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
            execute(get_process(cmd));
        }
    }
    destroy_list(positions);
}
