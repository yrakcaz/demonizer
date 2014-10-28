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
    while (str && str[0] && str[1])
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
        int fd = open("/dev/null", 0);
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        int ret = execvp(process[0], process);
        if (ret < 0)
        {
            fprintf(stderr, "dem: %s: an error has occurred\n", process[0]);
            exit(1);
        }
        exit(0);
    }
}

static void treat_kill(char **argv, int i)
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
            if (kill(deamons[atoi(argv[i + 1])], SIGKILL) != -1)
                printf("Deamon has been killed!\n");
            else
                perror(NULL);

        }
        else
            fprintf(stderr, "dem: this doesn't exist\n");
    }
    exit(0);
}

static void treat_restart(char **argv, int i)
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
            if (kill(deamons[atoi(argv[i + 1])], SIGKILL) == -1)
                perror(NULL);
            else
                printf("Deamon has been killed!\n");
            char **cmdline = str_to_wordtab(process);
            execute(cmdline);
            printf("Deamon has been restarted!\n");
            destroy_matrix(cmdline);
        }
        else
            fprintf(stderr, "dem: this doesn't exist\n");
    }
    exit(0);
}

static void treat_continue(char **argv, int i)
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
            if (kill(deamons[atoi(argv[i + 1])], SIGCONT) == -1)
                perror(NULL);
            else
                printf("Deamon has been continued!\n");
        }
        else
            fprintf(stderr, "dem: this doesn't exist\n");
    }
    exit(0);
}

static void treat_stop(char **argv, int i)
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
            if(kill(deamons[atoi(argv[i + 1])], SIGSTOP) == -1)
                perror(NULL);
            else
                printf("Deamon has been stopped!\n");
        }
        else
            fprintf(stderr, "dem: this doesn't exist\n");
    }
    exit(0);
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
                treat_kill(argv, i);
            else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--restart"))
                treat_restart(argv, i);
            else if (!strcmp(argv[i], "-cont") || !strcmp(argv[i], "--continue"))
                treat_continue(argv, i);
            else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--stop"))
                treat_stop(argv, i);
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
        printf("Deamon has been created!\n");
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
            printf("Deamon has been created!\n");
        }
    }
    destroy_list(positions);
}
