#include "../include/pid_file.h"

static char *itoa(int n)
{
    char *txt = malloc (16 * sizeof (char));
    sprintf(txt, "%d", n);
    return txt;
}


//delete pid has to be done!
/*static void delete_pid(int process)
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    char *str = malloc(4096 * sizeof (char));
    read(fd, str, 4096);
    close(fd);
    FILE *f = fopen(path, "w");
    for (int i = 0; str[i]; i++)
    {
        char *num = malloc(16 * sizeof (char));
        int j = i;
        if (!i || str[i] == '\n')
        {
            int j = 0;
            while (str[i] != '\t')
            {
                num[j] = str[i];
                j++;
                i++;
            }
            num[j] = '\0';
        }
        if (atoi(num) == process)
            while (str[i] != '\n')
                i++;
        else
        {
            while (str[i] != '\n')
            {
                fprintf(f, "%c", str[i]);
                i++;
            }
            fprintf(f, "%c", str[i]);
        }
        free (num);
    }
}*/

pid_t get_pid(int process)
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    char *str = malloc(4096 * sizeof (char));
    read(fd, str, 4096);
    int i = 0;
    while (str[i])
    {
        char *line = malloc (128 * sizeof (char));
        int j = 0;
        while (str[i] != '\t')
        {
            line[j] = str[i];
            i++;
            j++;
        }
        line[j] = '\0';
        if (atoi(line) == process)
        {
            i++;
            while (str[i] != '\t')
                i++;
            i++;
            j = 0;
            while (str[i] != '\n')
            {
                line[j] = str[i];
                i++;
                j++;
            }
            line[j] = '\0';
            return atoi(line);
        }
        while (str[i] != '\n')
            i++;
        i++;
        free(line);
    }
    return -1;
}

void set_pid(char *process, pid_t pid)
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    char *str = malloc(4096 * sizeof (char));
    read(fd, str, 4096);
    int i = 0;
    for (int j = 0; str[j]; j++)
        if (str[j] == '\n')
            i++;
    write(fd, itoa(i), strlen(itoa(i)));
    write(fd, "\t", 1);
    write(fd, process, strlen(process));
    write(fd, "\t", 1);
    write(fd, itoa(pid), strlen(itoa(pid)));
    write(fd, "\n", 1);
    close(fd);
}

void clean()
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    FILE *f = fopen(path, "w");
    fclose(f);
    exit(0);
}
