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
                    kill(deamons[atoi(argv[i + 1])], SIGKILL);
                }
                exit(0);
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
