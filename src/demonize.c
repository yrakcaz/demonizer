#include "../include/demonize.h"

static void display_help()
{
    printf("Usage: dem [option] [command] ...\n");
    printf("       --cmd, -c:\t\tif you have severals commands to demonize, use this option before each of them.\n");
    printf("       --kill, -k:\t\tkill the deamon corresponding to the next index.\n");
    printf("       --restart, -r:\t\trestart the deamon corresponding to the next index.\n");
    printf("       --stop, -s:\t\tstop the deamon corresponding to the next index.\n");
    printf("       --continue, -cont:\tcontinue the running of the deamon corresponding to the next index if it was stopped.\n");
    printf("       --jobs, -j:\t\tdisplay jobs.\n");
    printf("       --help, -h:\t\tdisplays this help.\n");
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
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        int k = 0;
        while (str[i] != ' ' && str[i] != '\0' && str[i] != '\t')
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
        {
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
            else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--kill"))
            {
                if (!(argv + i + 1) || !argv[i + 1])
                {
                    fprintf(stderr, "dem: use --help or -h option\n");
                    exit(1);
                }
                else
                {
                    get_deamons();
                    if (deamons[atoi(argv[i + 1])])
                        kill(deamons[atoi(argv[i + 1])], SIGKILL);
                    else
                        fprintf(stderr, "dem: this doesn't exist\n");
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
                {
                    get_deamons();
                    if (deamons[atoi(argv[i + 1])])
                    {
                        char *process = get_process_cmd(deamons[atoi(argv[i + 1])]);
                        kill(deamons[atoi(argv[i + 1])], SIGKILL);
                        execute(str_to_wordtab(process));
                    }
                    else
                        fprintf(stderr, "dem: this doesn't exist\n");
                }
                exit(0);
            }
            else if (!strcmp(argv[i], "-cont") || !strcmp(argv[i], "--continue"))
            {
                if (!(argv + i + 1) || !argv[i + 1])
                {
                    fprintf(stderr, "dem: use --help or -h option\n");
                    exit(1);
                }
                else
                {
                    get_deamons();
                    if (deamons[atoi(argv[i + 1])])
                        kill(deamons[atoi(argv[i + 1])], SIGCONT);
                    else
                        fprintf(stderr, "dem: this doesn't exist\n");
                }
                exit(0);
            }
            else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--stop"))
            {
                if (!(argv + i + 1) || !argv[i + 1])
                {
                    fprintf(stderr, "dem: use --help or -h option\n");
                    exit(1);
                }
                else
                {
                    get_deamons();
                    if (deamons[atoi(argv[i + 1])])
                        kill(deamons[atoi(argv[i + 1])], SIGSTOP);
                    else
                        fprintf(stderr, "dem: this doesn't exist\n");
                }
                exit(0);
            }
            else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cmd"))
                list = add_to_list(i + 1, list);
        }
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
        execute(cmd);
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
            execute(cmd);
        }
    }
    destroy_list(positions);
}
